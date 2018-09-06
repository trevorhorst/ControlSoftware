    .section .rodata
    .global resource_index_html
    .type   resource_index_html, @object
    .align  4
resource_index_html:
    .incbin "index.html"
resource_index_html_end:
    .global resource_index_html_size
    .type   resource_index_html_size, @object
    .align  4
resource_index_html_size:
    .int    resource_index_html_end - resource_index_html
