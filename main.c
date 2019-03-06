

#include "stm32f10x.h"
#include "lvgl/lvgl.h"
#include "ili9341/core.h"
#include "ili9341/lv_driver.h"
#include "misc/delay.h"
#include "XPT2046/XPT2046.h"

static lv_res_t list_btn_action(lv_obj_t* btn);
static lv_res_t btn_click_action(lv_obj_t* btn);

static lv_obj_t* list1;
static lv_obj_t* label1;
static lv_obj_t* btn1;
static lv_obj_t* label1;

void lv_test_list_1(void)
{
    list1 = lv_list_create(lv_scr_act(), NULL);
    lv_obj_set_pos(list1, 10, 10);
    lv_list_add(list1, SYMBOL_FILE, "File", list_btn_action);
    lv_list_add(list1, SYMBOL_DIRECTORY, "Directory", list_btn_action);
    lv_list_add(list1, SYMBOL_WIFI, "WiFi", list_btn_action);

    btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_pos(btn1, 220, 10);
    label1 = lv_label_create(btn1, NULL);
    lv_label_set_text(label1, "OK");
    lv_btn_set_action(btn1, LV_BTN_ACTION_CLICK, btn_click_action);

    label1 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label1, "Title Label");
    lv_obj_set_pos(label1, 10, 215);
}

int main()
{
    lv_init();

    ILI9341_init();
    lv_disp_drv_t disp;
    lv_disp_drv_init(&disp);
    disp.disp_flush = ILI9341_flush;
    lv_disp_drv_register(&disp);

    xpt2046_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = xpt2046_read;
    lv_indev_drv_register(&indev_drv);

    lv_test_list_1();

    for (;;) {
        delay_ms(10);
        lv_task_handler();
        lv_tick_inc(10);
    }
}

//******** PROGRAM NEVER REACH THIS FUNCTION *******
static lv_res_t list_btn_action(lv_obj_t* btn)
{
    lv_label_set_text(label1, lv_list_get_btn_text(btn));
    return LV_RES_OK;
}

static lv_res_t btn_click_action(lv_obj_t* btn)
{
    lv_label_set_text(label1, "OK");
    return LV_RES_OK;
}
