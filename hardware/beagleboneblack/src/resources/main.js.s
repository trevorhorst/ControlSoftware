    .section .rodata
    .global resource_main_js
    .type   resource_main_js, @object
    .align  4
resource_main_js:
    .incbin "main.js"
resource_main_js_end:
    .global resource_main_js_size
    .type   resource_main_js_size, @object
    .align  4
resource_main_js_size:
    .int    resource_main_js_end - resource_main_js
