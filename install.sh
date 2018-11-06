#!/bin/sh

original_loc=`pwd`/build/image_viewer
echo "copying $original_loc to /usr/local/bin"
sudo cp $original_loc /usr/local/bin

echo "generating image_viewer.desktop"
echo "[Desktop Entry]
Type=Application
Encoding=UTF-8
Name=image_viewer
Comment=image_viewer
Exec=/usr/local/bin/image_viewer %U

Terminal=false" > image_viewer.desktop

chmod 755 image_viewer.desktop

echo "copying image_viewer.desktop to /usr/share/applications"
sudo cp image_viewer.desktop /usr/share/applications
 
