# Qt画图板

# 实现方案一

使用`QPixmap`​, `QPen`​, `QPainter`​三者配合实现画图。

这样做可以实现在白板上画图的效果，但是对于在白板上我们所画的图形项，我们无法进行操作。

这不符合我们的预期，所以需要想新方法来实现自己期望中的画图板。

> 图形项无法自由地缩放、拖拽等操作。

要完成这种操作是否意味着图形项本身也可以被当做一个窗口？

> 可以参考老秦的WBoard项目如何实现Page中图形项的管理的。

‍
