#include "bclib.hh"
#include "platform.hh"

int print(S8 msg, PlatformFD fd)
{
    PlatformWritten pw = platform_write(&fd, msg);
    return pw.bytes_written;
}

int print(StrList * node, PlatformFD fd)
{
    int written = 0;
    while (node != nullptr) {
        written += print(*(node->v), fd);
        node = node->next;
    }

    return written;
}
