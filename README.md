### Build and Run Container

```
bash run_with_jupiter.sh
```

### Exec into the container

```
docker exec -it f3d0b83e80c3 bash
```

### This is not requried, keep it as a last resort if nothing works

```
./opt/nvidia/deepstream/deepstream/user_additional_install.sh
```

(takes about 12-15 mins)

### Install python apps (this installs pyds)

```
./user_deepstream_python_apps_install.sh --version master
```

### give access to python apps to allow changes from outside the container

```
chmod -R 777 deepstream_python_apps
```

#### Confirm the DeepStream GStreamer plugin files exist

```
ls -l /opt/nvidia/deepstream/deepstream/lib/gst-plugins | egrep "infer|nvinfer|nvdsgst"

gst-inspect-1.0 nvinfer
```

If you don’t see it, you’re likely in a container variant that doesn’t include the full DS GStreamer plugin set (wrong tag / incomplete install).

### Run the application

```
cd /opt/nvidia/deepstream/deepstream/sources/deepstream_python_apps/apps/deepstream-test1

python3 deepstream_test_1.py /opt/nvidia/deepstream/deepstream/samples/streams/sample_qHD.h264
```

### Further analysis

1. for pyds install bindings
   https://github.com/NVIDIA-AI-IOT/deepstream_python_apps/tree/master/bindings

2. change the dockerfile to not install Jupiterlab, but install it

...
python3 -m pip install --upgrade pip python3 -m pip install jupyterlab
jupyter lab --ip=0.0.0.0 --port=8888 --no-browser --allow-root --ServerApp.token='' --ServerApp.password=''
'''

http://127.0.0.4:8889/Lab

---

Notes:

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

docker push n0man/nvidia-deepstream:8.0-triton-multiarch-jupiter
