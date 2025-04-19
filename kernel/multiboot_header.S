/* multiboot header */
.section .multiboot
    .int 0x1BADB002 /* Magic number */
    .int 0x00       /* Flags */
    .int 0x00       /* Checksum */
