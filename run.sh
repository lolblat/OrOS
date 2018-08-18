#!/bin/sh

create_filesystem(){
  bximage -q -hd -mode=flat -size=50 disk boot/disk
  mkfs.ext2 boot/disk
  echo "Coping files"
  sudo mount boot/disk /mnt
  sudo cp filesystem/* /mnt
  sudo umount /mnt
}

run(){
  echo "Compile kernel and boot loader"
  make all
  success=$?
  if [ $success -ne 2 ]; then
    echo "Creating filesystem"
    create_filesystem
    cd boot/ && bochs
  fi
}
run
