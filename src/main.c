#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;  
static GFont s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // format the way that the time text is going to be displayed
strftime(buffer, sizeof("00:00"),"%I:%M", tick_time);


  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}


static void main_window_load(Window *window) {
  // Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SPARK_ICON);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 180));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  // Create time TextLayer
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ATE_BIT_24));

  //time layer
  s_time_layer = text_layer_create(GRect(15, 80, 120, 40));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorMediumAquamarine);
  text_layer_set_text(s_time_layer, "00:00");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
    
    // Unload GFont
    fonts_unload_custom_font(s_time_font);
    // Destroy time layer
    text_layer_destroy(s_time_layer);
    
}

//register all events
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
 update_time();
 
}

  


static void init() {
 
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Make sure the time is displayed from the start
  update_time();
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}


//this is the main road map for the app  first it will initialize by running init(), then it will run the app until
//the user exits the app and then will run the deinit() code, which will free up all of the memory that we 
//set aside at the for the app.
int main(void) {
  init();
  app_event_loop();
  deinit();
}