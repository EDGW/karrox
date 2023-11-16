def code(content):
    print((tab*"    ")+content)
def definepointer(name,target):
    code(name+" : "+target)
def definetarget(name):
    code(name+" : ")
def putnamedaddr(name,type,target:str):
    definepointer(name,"."+type+"  "+target);
def putnamednum(name,type,target:int):
    definepointer(name,"."+type+"  $"+target);
def putaddr(type,target:str):
    code("."+type+"  "+target);
def putnum(type,target:int):
    code("."+type+"  $"+target);
def globl(name):
    code(".globl "+name)
def comment(value):
    code("//"+value)
def section(value):
    code("\n\n.section "+value)


tab = 0

#intr table
code(".code32")
section(".data")
globl("intr_handler_table")
putnamedaddr("intr_handler_table","long",".iht")
definetarget(".iht")
irq_count = 0x30
for i in range(1,irq_count+1):
    putaddr("long","intr_handler"+str(i))
#intr table ends
#string table
section(".data")
putnamedaddr("intr_str","ascii","\"Intr handler!\\0\"")
#string table ends

tab+=1
for i in range(1,irq_count+1):
    section(".text")
    globl("intr_handler"+str(i))
    definetarget("intr_handler"+str(i))
    tab+=1
    code("push $intr_str")
    code("call asm_putstr")
    code("jmp .")
    tab-=1