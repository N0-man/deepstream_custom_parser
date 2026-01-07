
### Unnecessary packages
- python-gi-dev - old legacy gi dev package
- notebook==6.1.5, widgetsnbextension and jupyter_contrib_nbextensions - dont need classic notebooks
- drop PyGObject from pip. 
    - we already have python3-gi (+ python3-gst-1.0). `pip install PyGObject` will compile native code which is prblamatic and unnecessary
- audio codec dev pacages, not necessary
    - libflac-dev libdca-dev libdvdread-dev libmp3lame-dev libmpg123-dev libdvdnav-dev libmjpegutils-*
- remove redis-server
- remove install.sh

