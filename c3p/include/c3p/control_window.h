#include <gtk/gtk.h>

typedef struct ControlData
{
  int scale1;
} ControlData;

static void print_hello (GtkWidget *widget,
                         gpointer   data)
{
  g_print ("Hello World\n");
}

static void updateScale(GtkRange *scale, gpointer data, ControlData *ctl)
{
  printf("value: %d \n", gtk_range_get_value(scale));
//  ctl->scale1 = gtk_range_get_value(scale);
}

static void activate (GtkApplication* app, gpointer user_data, ControlData *ctl)
{
  GtkWidget* window;
  GtkWidget* grid;
  GtkWidget* button;
  GtkWidget* button_box;
  GtkWidget* scale;
  GtkWidget* scale2;

  //create new window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Particle System Controls");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  //construct container that will pack the widgets
  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 10, 1);
//  gtk_scale_set_draw_value(scale, 1);
 // gtk_container_add(GTK_CONTAINER(window), scale);
  g_signal_connect(scale, "value-changed", G_CALLBACK(updateScale), ctl);
  gtk_grid_attach(GTK_GRID(grid), scale, 1,0,10,1);

//  scale2 = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
//  gtk_grid_attach(GTK_GRID(grid), scale2, 2,0,2,1);
//  gtk_container_add(GTK_CONTAINER(window), scale2);

  //button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  //gtk_container_add(GTK_CONTAINER(window), button_box);

  //button = gtk_button_new_with_label ("Hello World");
  //g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
////  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  //gtk_container_add (GTK_CONTAINER (button_box), button);

  gtk_widget_show_all (window);
}
