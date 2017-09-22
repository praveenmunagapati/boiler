systemd 

Background reading:

 * http://0pointer.de/blog/projects/systemd.html
 * http://0pointer.de/blog/projects/socket-activation.html
 * http://0pointer.de/blog/projects/socket-activation2.html
 * http://0pointer.de/blog/projects/journal-submit.html

Some core ideas:

 * Service startup based on pre-creation of UNIX domain sockets and
   D-bus addresses, followed by parallel startup of services, so that
   service endpoints can be used (possibly blocking) while their 
   backing services start up. Also, on-demand startup (say CUPS,
   as it need only be started if the socket or bus service is used).
 * Device instantiation in the /dev tree can be a service depenendcy
 * mount points being mounted can induce service startup, or via autofs
   a process can access a filesystem (blocking) before its actually 
   mounted, at which point a backing mount can be conducted
 * processes are executed inside a cgroup
 * all logs (stdout, stderr) are collected from processes

An example service that exposes, say, a UNIX domain socket to which
a server process (daemon) binds and reads messages, would have two
systemd unit files in /etc/systemd/system (and systemctl daemon-reload
would make sure systemd has loaded them):

    /etc/systemd/system/example.socket
    /etc/systemd/system/example.service

This will make systemd itself create the UNIX domain socket, specified
in the .socket file. When a connection is made to it, systemd will 
start the service (daemon) for on-demand style startup. Or, the .service
file can cause the daemon to be loaded eagerly.

