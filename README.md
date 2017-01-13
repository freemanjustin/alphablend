# alphablend

Simple alpha blending code. Given 2 input images, alphablend will alpha blend the images. The alpha value is specified as a command line arg.

The blending equation is
```
alphaBlended = alpha*image1 + (1 - alpha)*image2
```

# usage
```
args: [input file1] [input file2] [output file] [alpha level 0-100]
```

