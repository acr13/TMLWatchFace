#include <pebble.h>

// Main Window
Window *window;

// Text Layers
TextLayer *text_date_layer;
TextLayer *text_time_layer;

// Top line
Layer *top_line;

// Leafs
GBitmap *leafs;
BitmapLayer *leafs_layer;

int WATCH_WIDTH = 144;
int WATCH_HEIGHT = 168;

void top_line_update_callback(Layer *layer, GContext* ctx)
{
	graphics_context_set_fill_color(ctx, GColorWhite);
	graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed)
{
	static char time_text[] = "00:00";
	static char date_text[] = "Xxxxxxxxx 00";
	
	char *time_format;
	
	// Conver the tick time to a date string 
	// Example: November 19
	strftime(date_text, sizeof(date_text), "%B %e", tick_time);
	text_layer_set_text(text_date_layer, date_text);

	// user clock setting
	if (clock_is_24h_style())
	{
		time_format = "%R";
	}
	else
	{
		time_format = "%I:%M";
	}
	
	strftime(time_text, sizeof(time_text), time_format, tick_time);
	
	if (time_text[0] == '0')
	{
        time_text[0] = time_text[1];
		time_text[1] = time_text[2];
		time_text[2] = time_text[3];
		time_text[3] = time_text[4];
		time_text[4] = time_text[5];
	}
	
	text_layer_set_text(text_time_layer, time_text);
}

// Init function to handle the creation of layers,
// event subscribing, etc
void handle_init(void) 
{
	// window init
	window = window_create();
	window_stack_push(window, true);
	window_set_background_color(window, GColorWhite);

	// display layer
	Layer *window_layer = window_get_root_layer(window);
	
	// background
	leafs = gbitmap_create_with_resource(RESOURCE_ID_BKNG);
	leafs_layer = bitmap_layer_create(GRect(0, 0, WATCH_WIDTH, WATCH_HEIGHT));
	bitmap_layer_set_bitmap(leafs_layer, leafs);
	layer_add_child(window_layer, bitmap_layer_get_layer(leafs_layer));
	
	GFont prox_reg = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PROXIMA_REGULAR_16));
	GFont prox_bold = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PROXIMA_BOLD_30));

	// time init
	text_time_layer = text_layer_create(GRect(55, 100,(WATCH_WIDTH - 55), 50));
	text_layer_set_font(text_time_layer, prox_bold);
	text_layer_set_background_color(text_time_layer, GColorClear);
    layer_add_child(window_layer, text_layer_get_layer(text_time_layer));
	
	// date init
	text_date_layer = text_layer_create(GRect(55, 130, (WATCH_WIDTH - 55), 20));
	text_layer_set_font(text_date_layer, prox_reg);
	text_layer_set_background_color(text_date_layer, GColorClear);
    layer_add_child(window_layer, text_layer_get_layer(text_date_layer));
	
	/*
	// top-line init
	GRect line_frame = GRect(0, 10, WATCH_WIDTH, 2);
	top_line = layer_create(line_frame);
	layer_set_update_proc(top_line, top_line_update_callback);
	layer_add_child(window_layer, top_line);
	*/
		
	// Avoids a blank screen on watch start.
  	time_t now = time(NULL);
  	struct tm *tick_time = localtime(&now);

 	//update_display(tick_time);
	tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}


void handle_deinit(void) 
{
	layer_remove_from_parent(bitmap_layer_get_layer(leafs_layer));
  	bitmap_layer_destroy(leafs_layer);
 	gbitmap_destroy(leafs);
	
	tick_timer_service_unsubscribe();
	
	window_destroy(window);
}

int main(void) 
{
	handle_init();
	app_event_loop();
	handle_deinit();
}