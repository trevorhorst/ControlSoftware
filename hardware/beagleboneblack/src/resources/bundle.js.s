    .section .rodata
    .global BUNDLE_JS
    .type   BUNDLE_JS, %object
    .align  4
BUNDLE_JS:
    .incbin "bundle.js"
BUNDLE_JS_END:
    .global BUNDLE_JS_SIZE
    .type   BUNDLE_JS_SIZE, %object
    .align  4
BUNDLE_JS_SIZE:
    .int    BUNDLE_JS_END - BUNDLE_JS
