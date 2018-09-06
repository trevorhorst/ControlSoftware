    .section .rodata
    .global ask
    .type   ask, @object
    .align  4
ask:
    .incbin "resources/ask.html"
ask_end:
    .global ask_size
    .type   ask_size, @object
    .align  4
ask_size:
    .int    ask_end - ask
