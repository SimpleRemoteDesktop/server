# server
SimpleRemoteDesktop screen sharing server


## install dependency

### required

```
apt-get install libboost-dev libxtst-dev libavutil-dev libavcodec-dev libswscale-dev libx264-dev libboost-thread-dev libboost-dev cmake libboost-log1.58-dev libopus-dev
```

### option NVENC

You must install cuda SDK from Nvidia web site

## configuration

```
cmake . // software encoder only
cmake --WITH_NVENC // Nvidia NVENC encoder support 

make . // build
```

## run application

```
./src/SRDServer 
// or with nivida encoder
./src/SRDServer +nvenc
```

