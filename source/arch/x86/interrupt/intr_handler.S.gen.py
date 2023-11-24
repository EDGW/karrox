excpcnt = 0x30
err_codes = [8,10,11,12,13,14,17,21,29,30]
print(".extern intr_handler_table")
print("""
.section .data
.global intr_original_handlers
intr_original_handlers:
""")
for i in range(excpcnt):
    s = ""
    if not i in err_codes:
        s="pushl $0"
    print(f"""
.section .text
    .type intr_original_handler_{i}, @function
    intr_original_handler_{i}:
          {s}
          push %ds
          push %es
          push %fs
          push %gs
          pusha
          pushl ${i}     // push intr number
          movl $intr_handler_table,%ebx
          call *{i*4}(%ebx)
          add $4,%esp   // skip intr number
          popa
          pop %gs
          pop %fs
          pop %es
          pop %ds
          add $4,%esp   // skip error-code
          iret          // return from interrupt


.section .data
    .long intr_original_handler_{i}

""");