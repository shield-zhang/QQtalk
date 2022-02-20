#ifndef IMAGECHANGE_H
#define IMAGECHANGE_H
class Images_Change
{
public:
    Images_Change();
    char *NextImage();
    /*图片的路径*/
    char* images[6] = {":/1.jpg",":/2.jpg",":/3.jpg",":/4.jpg",":/5.jpg",":/6.jpg"};
    int index;
};


#endif // IMAGECHANGE_H
