### Build image and and run container (tested locally on RTX 5090)

```
bash run_with_jupyter.sh
```

### Exec into the container

```
docker exec -it f3d0b83e80c3 bash
```

### Kill all containers

```
bash kill.sh
```

### publish to docker hub

```
docker login

docker push n0man/nvidia-deepstream:8.0-triton-multiarch-jupyter
```

### test the application

```
cd /opt/nvidia/deepstream/deepstream/sources/deepstream_python_apps/apps/deepstream-test1

python3 deepstream_test_1.py /opt/nvidia/deepstream/deepstream/samples/streams/sample_qHD.h264
```

### Further analysis

#### Confirm the DeepStream GStreamer plugin files exist

```
ls -l /opt/nvidia/deepstream/deepstream/lib/gst-plugins | egrep "infer|nvinfer|nvdsgst"

gst-inspect-1.0 nvinfer
```

If you don’t see it, you’re likely in a container variant that doesn’t include the full DS GStreamer plugin set (wrong tag / incomplete install).

#### Notes

- dont point to pyds folder, its not the true python bindings
- dont do pip3 install pyds - taht is not deepstream python bindings

In the DeepStream container, the plugins are typically under:

- Plugins (.so): /opt/nvidia/deepstream/deepstream/lib/gst-plugins/
- DS libs: /opt/nvidia/deepstream/deepstream/lib/

#### Confirm the DeepStream GStreamer plugin files exist

`ls -l /opt/nvidia/deepstream/deepstream/lib/gst-plugins | egrep "infer|nvinfer|nvdsgst"`
You should see something like libnvdsgst_infer.so (or similarly named).

If you don’t see it, you’re likely in a container variant that doesn’t include the full DS GStreamer plugin set (wrong tag / incomplete install).

#### Force GStreamer to rescan with correct paths (and wipe cache)

rm -f ~/.cache/gstreamer-1.0/registry.\* 2>/dev/null

export LD_LIBRARY_PATH=/opt/nvidia/deepstream/deepstream/lib:$LD_LIBRARY_PATH
export GST_PLUGIN_PATH=/opt/nvidia/deepstream/deepstream/lib/gst-plugins:$GST_PLUGIN_PATH

gst-inspect-1.0 nvinfer