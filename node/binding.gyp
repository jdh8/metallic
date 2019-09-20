{
    "targets": [
        {
            "target_name": "syscalls", 
            "defines": [
                "_FILE_OFFSET_BITS=64",
                "NAPI_EXPERIMENTAL"
            ],
            "sources": [
                "fs/fallocate.c",
                "fs/fchdir.c",
                "fs/fcntl.c",
                "fs/seek.c",
                "syscalls.c"
            ]
        }
    ]
}
