# KindleStar

![KindleStar Orion](https://cdn.shopifycdn.net/s/files/1/0613/4631/6544/files/e8521354352c20aa92dc80bc2bf1b9d6_100x@2x.png?v=1637820493)

A customizable 75% keyboard.

* Keyboard Maintainer: [KindleStar](https://kindlestar.online)
* Hardware Supported: KindleStar Orion
* Hardware Availability: [KindleStar](https://kindlestar.online)

First update git submodule
```make git-submodule```

Make example for this keyboard (after setting up your build environment):

    make kindlestar/oreo:via

Flashing example for this keyboard:

    make kindlestar/oreo:via flash

**Reset Key**: Hold down the key located at *K00*, commonly programmed as *Esc* while plugging in the keyboard.

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

```
via教程
```

"matrix": {"rows": 6, "cols": 4}
代表矩阵行列，必须改

layouts
要在这里生成：
http://www.keyboard-layout-editor.com/#/

然后标记layout
给他们加上坐标， 这些坐标和layout的kxxx 要保持一致

pad的layout是：
https://gist.github.com/79af54f45233b17ee71fae57b8868e30

