#include "bclib.hh"
#include "platform.hh"

int print(S8 msg, PlatformFD fd)
{
    PlatformWritten pw = platform_write(&fd, msg);
    return pw.bytes_written;
}

int print(StrList list, PlatformFD fd)
{
    StrList * node = &list;
    int written = 0;
    while (node != nullptr) {
        written += print(node->val, fd);
        node = node->next;
    }

    return written;
}

void strlist_append(StrList * node, Arena * arena, S8 str)
{
    while (node->next != nullptr) {
        node = node->next;
    }
    
    node->val = str;
    node->next = make<StrList>(arena);
}
