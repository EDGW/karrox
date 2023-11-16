def code(content):
    print((tab*"    ")+content)
def definepointer(name,target):
    code(name+": "+target)
def definetarget(name):
    code(name+": ")
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
code('#include \"interrupt_asm.h\"')
section(".data")
globl("intr_handler_table")
definetarget("intr_handler_table")
irq_count = 0x30
for i in range(irq_count):
    putaddr("long","hand_intr_handler"+str(i))
#intr table ends
#string table
section(".data")
putnamedaddr("intr_str","ascii","\"Intr handler!\\0\"")
#string table ends

desc = ["EXCP 0x0 (#DE) Division Error ",
    "EXCP 0x1 (#DB) Debug ",
    "Non-maskable Interrupt ",
    "EXCP 0x3 (#BP) Breakpoint ",
    "EXCP 0x4 (#OF) Overflow ",
    "EXCP 0x5 (#BR) Bound Range Exceeded ",
    "EXCP 0x6 (#UD) Invalid Opcode ",
    "EXCP 0x7 (#NM) Device Not Available ",
    "EXCP 0x8 (#DF) Double Fault ",
    "Coprocessor Segment Overrun ",
    "EXCP 0xA (#TS) Invalid TSS ",
    "EXCP 0xB (#NP) Segment Not Present ",
    "EXCP 0xC (#SS) Stack-Segment Fault ",
    "EXCP 0xD (#GP) General Protection Fault ",
    "EXCP 0xE (#PF) Page Fault ",
    "Reserved ",
    "EXCP 0x10 (#MF) x87 Floating-Point Exception ",
    "EXCP 0x11 (#AC) Alignment Check ",
    "EXCP 0x12 (#MC) Machine Check ",
    "EXCP 0x13 (#XM/#XF) SIMD Floating-Point Exception ",
    "EXCP 0x14 (#VE) Virtualization Exception ",
    "EXCP 0x15 (#CP) Control Protection Exception ",
    "Reserved ",
    "Reserved ",
    "Reserved ",
    "Reserved ",
    "Reserved ",
    "Reserved ",
    "EXCP 0x1C (#HV) Hypervisor Injection Exception ",
    "EXCP 0x1D (#VC) VMM Communication Exception ",
    "EXCP 0x1E (#SX) Security Exception ",
    "Reserved ",
    "0x20 (INT) User Defined Interrupt",
    "0x21 (INT) User Defined Interrupt",
    "0x22 (INT) User Defined Interrupt",
    "0x23 (INT) User Defined Interrupt",
    "0x24 (INT) User Defined Interrupt",
    "0x25 (INT) User Defined Interrupt",
    "0x26 (INT) User Defined Interrupt",
    "0x28 (INT) User Defined Interrupt",
    "0x29 (INT) User Defined Interrupt",
    "0x25 (INT) User Defined Interrupt",
    "0x2A (INT) User Defined Interrupt",
    "0x2B (INT) User Defined Interrupt",
    "0x2C (INT) User Defined Interrupt",
    "0x2D (INT) User Defined Interrupt",
    "0x2E (INT) User Defined Interrupt",
    "0x2F (INT) User Defined Interrupt"]

errcode = [False]*0x30
errcode[0x8]=errcode[0xA]=errcode[0xB]=errcode[0xC]=errcode[0xD]=errcode[0xE] = True
errcode[0x11]=errcode[0x15]=errcode[0x1D]=errcode[0x1E]=True

tab+=1
for i in range(irq_count):
    section(".text")
    globl("hand_intr_handler"+str(i))
    definetarget("hand_intr_handler"+str(i))
    tab+=1
    if not errcode[i]:  # without errorcode
        code("push $0")
    code("push $intr_desc"+str(i))
    code("call asm_putstr")
    code("add $4,%esp") # argument of 'asm_putstr'

    code("movb $EOI,%al")
    code("outb %al,$X8259A_MASTER_1_PORT")
    code("outb %al,$X8259A_SLAVE_1_PORT")
    code("add $4,%esp") #error code
    code("iret")  #return from interrupt
    section(".data")
    putnamedaddr("intr_desc"+str(i),"ascii","\""+desc[i]+"\\n\\0\"")
    tab-=1
tab-=1