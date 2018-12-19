#ifndef MY_UTILS_H
#define MY_UTILS_H

float get_x_normalized(unsigned int x, const int scr_width);
float get_y_normalized(unsigned int y, const int scr_height);
int get_org_x(float x, const int scr_width);
int get_org_y(float y, const int scr_height);

float get_x_normalized(unsigned int x, const int scr_width)
{
    float val = 0;

    if (x > scr_width / 2)
        val = (x - scr_width / 2) / ((float)(scr_width / 2));
    else
        val = -(1 - (x / ((float)(scr_width / 2))));

    return val;
}

float get_y_normalized(unsigned int y, const int scr_height)
{
    float val = 0;
    if (y < scr_height / 2)
        val = 1 - (y / ((float)(scr_height / 2)));
    else
        val = -((y - scr_height / 2) / ((float)(scr_height / 2)));

    return val;
}

int get_org_x(float x, const int scr_width)
{
    int val = 0;
    if (x > 0)
        val = scr_width / 2 + (x * scr_width / 2);
    else
        val = -(x * scr_width / 2);

    return val;
}

int get_org_y(float y, const int scr_height)
{
    int val = 0;
    if (y > 0)
        val = y * scr_height / 2;
    else
        val = -(scr_height / 2 + (y * scr_height / 2));

    return val;
}

#endif