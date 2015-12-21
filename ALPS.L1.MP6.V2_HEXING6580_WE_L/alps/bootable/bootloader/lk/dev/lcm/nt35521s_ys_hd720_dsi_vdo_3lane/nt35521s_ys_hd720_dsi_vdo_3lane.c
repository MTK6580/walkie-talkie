#ifndef BUILD_LK
#include <linux/kernel.h>
#include <linux/string.h>
#endif

#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1280)

#define REGFLAG_DELAY             							0xFFFE
#define REGFLAG_END_OF_TABLE      							0xFFFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE									0

#define NT35521_LCM_ID				0x5521

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)									lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)				lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size) 


static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] = 
{

	//NVT internal command
	//{0xFF,4, {0xAA,0x55,0x25,0x01}},
	//{0x6F,1, {0x1A}},
	//{0xF7,1, {0x05}},
	
	{0xFF,4, {0xAA,0x55,0xA5,0x80}},
	{0x6F,1, {0x17}},
	{0xF4,1, {0x60}},

	// CMD2 page0 enable
	{0xF0,5, {0x55,0xAA,0x52,0x08,0x00}},

	// SDT time
	{0xB6,1, {0x06}},

	// Source EQ time
	{0x6F,1, {0x02}},
	{0xB8,1, {0x01}},

	// Source Bias time
	{0xBB,2, {0x11,0x11}},

	// Source Inversion
	{0xBC,2, {0x00,0x00}},

	// Dsiplay timing control
	{0xBD,5, {0x01,0xA0,0x10,0x10,0x01}},

	{0xC8,1, {0x80}},

	// CMD2 page1 enable
	{0xF0,5, {0x55,0xAA,0x52,0x08,0x01}},

	// VGH voltage (15V)
	{0xB3,2, {0x28,0x28}},

	// VGL volateg(-10V)
	{0xB4,2, {0x0F,0x0F}},

	// VGH pump ratio
	{0xB9,2, {0x35,0x35}},

	// VGL pump ratio
	{0xBA,2, {0x25,0x25}},

	// VGMP = 4.5V, VGMN = -4.5V, VGSP/VGSN = 0V
	{0xBC,2, {0x90,0x00}},

	{0xBD,2, {0x90,0x00}},

	// VCOM = -1.4375V
	{0xBE,1, {0x6C}},//0x60

	// EXTP/EXTN control
	{0xC0,1, {0x0C}},
	{0xCA,1, {0x00}},

	// CMD2 page2 enable
	{0xF0,5, {0x55,0xAA,0x52,0x08,0x02}},

	// Gamma voltage
	{0xEE,1, {0x01}},
	{0xB0,16, {0x00,0x00,0x00,0x1B,0x00,0x4B,0x00,0x6C,0x00,0x87,0x00,0xB1,0x00,0xD2,0x01,0x06}},

	{0xB1,16, {0x01,0x32,0x01,0x76,0x01,0xAC,0x02,0x03,0x02,0x49,0x02,0x4C,0x02,0x8F,0x02,0xD4}},

	{0xB2,16, {0x02,0xFB,0x03,0x30,0x03,0x52,0x03,0x7D,0x03,0x99,0x03,0xBD,0x03,0xD2,0x03,0xE8}},

	{0xB3,4, {0x03,0xFD,0x03,0xFF}},

	// CMD2 page3 enable
	{0xF0,5, {0x55,0xAA,0x52,0x08,0x03}},


	{0xF0,5, {0x55,0xAA,0x52,0x08,0x05}},
	{0x6F,1, {0x02}},
	{0xBD,1, {0x0F}},
	{0xC2,1, {0xA1}},
	{0xD1,5, {0x03,0x00,0x3D,0x07,0x10}},
	{0xD2,5, {0x03,0x00,0x3D,0x07,0x10}},

	// CMD2 page6 enable
	{0xF0,5, {0x55,0xAA,0x52,0x08,0x06}},

	// GOA setting3
	{0xB0,2, {0x00,0x10}},
	{0xB1,2, {0x12,0x16}},
	{0xB2,2, {0x18,0x31}},
	{0xB3,2, {0x31,0x31}},
	{0xB4,2, {0x31,0x34}},
	{0xB5,2, {0x34,0x34}},
	{0xB6,2, {0x34,0x31}},

	{0xB7,2, {0x31,0x31}},
	{0xB8,2, {0x31,0x2D}},
	{0xB9,2, {0x2E,0x04}},
	{0xBA,2, {0x05,0x2E}},
	{0xBB,2, {0x2D,0x31}},
	{0xBC,2, {0x31,0x31}},
	{0xBD,2, {0x31,0x34}},
	{0xBE,2, {0x34,0x34}},
	{0xBF,2, {0x34,0x31}},
	{0xC0,2, {0x31,0x31}},
	{0xC1,2, {0x31,0x19}},
	{0xC2,2, {0x17,0x13}},
	{0xC3,2, {0x11,0x01}},
	{0xC4,2, {0x05,0x19}},
	{0xC5,2, {0x17,0x13}},
	{0xC6,2, {0x11,0x31}},
	{0xC7,2, {0x31,0x31}},
	{0xC8,2, {0x31,0x34}},
	{0xC9,2, {0x34,0x34}},
	{0xCA,2, {0x34,0x31}},
	{0xCB,2, {0x31,0x31}},
	{0xCC,2, {0x31,0x2E}},
	{0xCD,2, {0x2D,0x01}},
	{0xCE,2, {0x00,0x2D}},
	{0xCF,2, {0x2E,0x31}},
	{0xD0,2, {0x31,0x31}},
	{0xD1,2, {0x31,0x34}},
	{0xD2,2, {0x34,0x34}},
	{0xD3,2, {0x34,0x31}},
	{0xD4,2, {0x31,0x31}},
	{0xD5,2, {0x31,0x10}},
	{0xD6,2, {0x12,0x16}},
	{0xD7,2, {0x18,0x04}},
	{0xD8,5, {0x00,0x00,0x00,0x00,0x00}},
	{0xD9,5, {0x00,0x00,0x00,0x00,0x00}},
	{0xE5,2, {0x31,0x31}},
	{0xE6,2, {0x31,0x31}},
	{0xE7,1, {0x00}},

	{0x11, 0,{0x00}},
	{REGFLAG_DELAY, 120, {}},
	{0x29, 0,{0x00}},
	{0x35, 0,{0x00}},
	{REGFLAG_DELAY, 50, {}},


	// Setting ending by predefined flag
	{REGFLAG_END_OF_TABLE, 0x00, {}}

};

static struct LCM_setting_table lcm_sleep_out_setting[] = 
{
    // Sleep Out
    {0x11, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
    {0x29, 1, {0x00}},
    {REGFLAG_DELAY, 50, {}},
    
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = 
{
	{0x28, 1, {0x00}},
	{REGFLAG_DELAY, 20, {}},

    // Sleep Mode On
    {0x10, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

    for(i = 0; i < count; i++) 
	{
		
        unsigned cmd;
        cmd = table[i].cmd;
		
        switch (cmd) {
			
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
				
            case REGFLAG_END_OF_TABLE :
                break;
				
            default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }
	
}


// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS));

    params->type   = LCM_TYPE_DSI;
    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;

	params->physical_width = 68;
	params->physical_height = 120;

    // enable tearing-free
    params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
    params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
    params->dsi.mode   = CMD_MODE;
    params->dsi.switch_mode = SYNC_PULSE_VDO_MODE;
#else
    params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif

    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM				= LCM_THREE_LANE;
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

    // Highly depends on LCD driver capability.
    // Not support in MT6573
    params->dsi.packet_size=256;
    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
	
	params->dsi.PLL_CLOCK = 250; //this value must be in MTK suggested table

	params->dsi.vertical_sync_active  = 2;
	params->dsi.vertical_backporch    = 14;
	params->dsi.vertical_frontporch   = 16;
	params->dsi.vertical_active_line  = FRAME_HEIGHT;


	params->dsi.horizontal_sync_active = 8;
	params->dsi.horizontal_backporch  = 24;
	params->dsi.horizontal_frontporch = 32;
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;

	params->dsi.HS_TRAIL = 120;

	params->dsi.clk_lp_per_line_enable = 0;
	params->dsi.esd_check_enable = 0;
	params->dsi.customization_esd_check_enable = 0;
	params->dsi.lcm_esd_check_table[0].cmd          = 0x53;
	params->dsi.lcm_esd_check_table[0].count        = 1;
	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x24;
}


static void lcm_init(void)
{
    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(100);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);

}


static unsigned int lcm_compare_id(void);
static void lcm_resume(void)
{
//	lcm_compare_id();//test_id	
	
	lcm_init();
}

#if (LCM_DSI_CMD_MODE)
static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0>>8)&0xFF);
	unsigned char x0_LSB = (x0&0xFF);
	unsigned char x1_MSB = ((x1>>8)&0xFF);
	unsigned char x1_LSB = (x1&0xFF);
	unsigned char y0_MSB = ((y0>>8)&0xFF);
	unsigned char y0_LSB = (y0&0xFF);
	unsigned char y1_MSB = ((y1>>8)&0xFF);
	unsigned char y1_LSB = (y1&0xFF);

	unsigned int data_array[16];

	data_array[0]= 0x00053902;
	data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
	data_array[2]= (x1_LSB);
	data_array[3]= 0x00053902;
	data_array[4]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[5]= (y1_LSB);
	data_array[6]= 0x002c3909;

	dsi_set_cmdq(&data_array, 7, 0);

}
#endif

static unsigned int lcm_compare_id(void)
{
	unsigned int id1 = 0,id2 = 0;
	unsigned int lcm_id = 0;
	unsigned char buffer[5];
	unsigned int array[8];

	SET_RESET_PIN(1);
	MDELAY(1);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(100);

	array[0] = 0x00063902;
	array[1] = 0x52AA55F0;
	array[2] = 0x00000108;
	dsi_set_cmdq(array, 3, 1);
	MDELAY(2);
	
	array[0] = 0x00023700;// return byte number
	dsi_set_cmdq(array, 1, 1);
	MDELAY(2);

	read_reg_v2(0xC5, buffer, 2);
	id1 = buffer[0];	
	id2 = buffer[1];

	lcm_id = ((id1<<8) | id2);

#ifdef BUILD_LK
	printf("LK,NT35521C: id1=0x%x,id2=0x%x\n",id1,id2);   
#else
	printk("KERNEL,NT35521C: id1=0x%x,id2=0x%x\n",id1,id2);  
#endif

	return (NT35521_LCM_ID == lcm_id)?1:0;
}


LCM_DRIVER nt35521s_ys_hd720_dsi_vdo_3lane_lcm_drv = 
{
    .name			= "nt35521s_ys_hd720_dsi_vdo_3lane",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
	.compare_id     = lcm_compare_id,
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
};
