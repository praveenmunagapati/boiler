Simple on-demand service

We'll install example.socket and start it. We also install example.service.
Systemd will establish the socket in a listening state, and will start the
associated example.service when we make a connection to the socket.

    % make
    % sudo cp srv /usr/bin/example
    % sudo cp example.socket /etc/systemd/system
    % sudo cp example.service /etc/systemd/system
    % sudo systemctl daemon-reload
    % sudo systemctl start example.socket

Note the appearance of 

    /tmp/example.sock

in the filesystem. Systemd created it and is polling on connection to it.

This example service is using on-demand startup.  Notice the
absence of WantedBy in the example.service. Instead a connection to
example.socket automatically starts example.service.

    %  ./cli -f /tmp/example.sock
    %  sudo systemctl status example.service

This particular server (/usr/bin/example) is coded to drain the client input
and then exit. It relies on systemd to restart if a new connection is made.

Shut it down:

    % sudo systemctl stop example.socket example.service
