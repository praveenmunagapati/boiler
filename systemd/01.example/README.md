We'll use a test program (srv) to consume from the activated socket, and
another one (cli) to connect to it.

    % make
    % sudo cp srv /usr/bin/example

The installation prefix /usr/bin has to match what is in the .service file.

    % sudo cp example.service example.socket /etc/systemd/system
    % sudo systemctl daemon-reload
    % systemctl start example.socket

Note appearance of 

    /tmp/example.sock

in the filesystem. Also, because this example service is using on-demand
startup- (notice the absence of WantedBy in the example.service), there is 
no example process running. But if you connect to the example socket, the
daemon gets started.

    %  ./cli /tmp/example.sock
    % ps aux | grep example

If "example" exits then systemd restarts it only when there is a new connection
to the UNIX domain socket.
