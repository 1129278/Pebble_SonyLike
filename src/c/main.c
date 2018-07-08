#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_timeHour_layer;
static TextLayer *s_timeMins_layer;

// Declare a file-scope variable
static GFont interstateRegular48_font;
static GFont interstateThin48_font;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours into a buffer
  static char s_bufferHour[4];
  strftime(s_bufferHour, sizeof(s_bufferHour), clock_is_24h_style() ? "%H" : " %I", tick_time);
  
  // Display this time on the TextLayer
  text_layer_set_text(s_timeHour_layer, s_bufferHour);

  // Write the current mins into a buffer
  static char s_bufferMins[4];
  strftime(s_bufferMins, sizeof(s_bufferMins), clock_is_24h_style() ? "%M" : " %M", tick_time);
  
  // Display mins on the TextLayer
  text_layer_set_text(s_timeMins_layer, s_bufferMins); 
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // Load the custom font
  interstateRegular48_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_INTERSTATE_REGULAR_48));
  interstateThin48_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_INTERSTATE_THIN_48));

  // Create the TextLayer with specific bounds (hour)
  s_timeHour_layer = text_layer_create(GRect(0, bounds.size.h/2 - 51, bounds.size.w, 50));

  // Improve the layout to be more like a watchface (hour)
  text_layer_set_background_color(s_timeHour_layer, GColorBlack);
  text_layer_set_text_color(s_timeHour_layer, GColorWhite);
  text_layer_set_text(s_timeHour_layer, "00");
  text_layer_set_font(s_timeHour_layer, interstateRegular48_font);
  text_layer_set_text_alignment(s_timeHour_layer, GTextAlignmentCenter);
  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_timeHour_layer));
  
  // Create the TextLayer with specific bounds (mins)
  s_timeMins_layer = text_layer_create(GRect(0, bounds.size.h/2 + 5, bounds.size.w, 50));

  // Improve the layout to be more like a watchface (mins)
  text_layer_set_background_color(s_timeMins_layer, GColorBlack);
  text_layer_set_text_color(s_timeMins_layer, GColorCyan);
  text_layer_set_text(s_timeMins_layer, "00");
  text_layer_set_font(s_timeMins_layer, interstateThin48_font);
  text_layer_set_text_alignment(s_timeMins_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_timeMins_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_timeHour_layer);
  text_layer_destroy(s_timeMins_layer);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  // Set background color
  window_set_background_color(s_main_window, GColorBlack);

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

int main(void) {
  init();
  app_event_loop();
  deinit();
}