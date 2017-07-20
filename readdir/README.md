Show usage of readdir(3) to iterate over a directory.

The directory entry has a field called d_type which
seems useful in distinguishing a regular file from a
directory or block device, fifo or other kind of file.
The readdir program prints a string representation of 
this field.

However, on some filesystems d_type is not populated.
Furthermore on systems like RHEL where the file may
have a security context, it may be reported as having
type DT_UNKNOWN (rather than, say, DT_DIR, for a 
directory having a security context). The security 
context is indicated by a trailing dot in the mode bits
from typical ls output e.g.

    % ls -l
    drwxrwxrwx. 8 root root 83 Jul 19 00:31 20170719

To correct the deficiency of the readdir dent->d_type
since it cannot be relied on robustly, we can do a call
to lstat(2) on each directory entry to get a more robust
set of mode bits. This is shown in readdir-lstat.c.

