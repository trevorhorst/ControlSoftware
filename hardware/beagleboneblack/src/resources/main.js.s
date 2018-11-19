    .section .rodata
    .global MAIN_JS
    .type   MAIN_JS, @object
    .align  4
MAIN_JS:
    .incbin "main.js"
MAIN_JS_END:
    .global MAIN_JS_SIZE
    .type   MAIN_JS_SIZE, @object
    .align  4
MAIN_JS_SIZE:
    .int    MAIN_JS_END - MAIN_JS
