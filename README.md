# JOSLINK
Jack over Steam Link

Basically this program allows you to patch Jack to the SteamLink Audio interface, this allows for example audio streaming over the network to the SteamLink

## Usage

Currently there is no GUI yet, and it's purely CLI, so you're not really able to boot it via the SteamLink UI, and you need SSH enabled for this

On your host machine
```bash
cd [joslink dir]
mkdir dist
cd dist
cmake ..
make && make install
cd [steamlink rootfs]/home/apps
scp -r joslink root@[steamlink host]:/home/apps
```

Now on the steam link setup your jack environment
e.g:

```bash
cd /home/apps/joslink;
export LD_LIBRARY_PATH=/home/apps/joslink/lib;
./bin/jackd -d netone &
./joslink &
./bin/jack_connect system:capture_[n] JOSLINK:channel_[n]
```

## TODO

* Create QT application
* Create auto-patcher (connect channels automatically with jack capture points)

## Credits

[jopa-ng](https://github.com/cjxgm/jopa-ng): very simple implementation which made using Jack very easy
