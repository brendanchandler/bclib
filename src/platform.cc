#include <unistd.h>
#include <errno.h>
#include "bclib.hh"

PlatformFD platform_stdout()
{
    PlatformFD pfd = {0};
    pfd.fd = 1;
    return pfd;
}

PlatformFD platform_stderr()
{
    PlatformFD pfd = {0};
    pfd.fd = 2;
    return pfd;
}

PlatformWritten platform_write(PlatformFD * pfd, S8 msg)
{
    PlatformWritten pw;
    int rc;

    while (pw.bytes_written < msg.len) {
        rc = write(pfd->fd, msg.val + pw.bytes_written, msg.len - pw.bytes_written);
        if (rc > 0) {
            pw.bytes_written += rc;
        } else if (rc == EAGAIN) {
            continue;
        } else if (rc == EINTR) {
            continue;
        } else {
            return pw;
        }
    }

    pw.ok = true;
    return pw;
}
