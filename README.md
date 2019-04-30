# server
SimpleRemoteDesktop screen sharing server


## install dependency

### required

```
apt-get install build-essential libboost-dev libxtst-dev libavutil-dev libavcodec-dev libswscale-dev libx264-dev libboost-thread-dev libboost-dev cmake libboost-log1.58-dev libopus-dev libpulse-dev libsdl2-dev 
```
## option Turbo-JPEG

require yasm to compile code
https://svwh.dl.sourceforge.net/project/libjpeg-turbo/2.0.2/libjpeg-turbo-official_2.0.2_amd64.deb
### option NVENC

You must install cuda SDK from Nvidia web site

## configuration

```
cmake . // software encoder only
cmake -DWITH_NVENC=ON . // Nvidia NVENC encoder support 
cmake -DWITH_TJPEG=ON . // turbojpeg encoder support
make // build
```

## run application

```
./src/SRDServer 
// or with nivida encoder
./src/SRDServer +nvenc
```

