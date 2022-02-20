#include "imagechange.h"

/*构造函数*/
Images_Change::Images_Change()
{
    index = -1;
}
/*切换图片的方法*/
char* Images_Change::NextImage(){
    if(index>=5)    //六张图片显示完后，就把index归0，重新开始显示第一张图片
        index=0;
    else
        index++;
    return(images[index]);  //返回图片的路径
}
