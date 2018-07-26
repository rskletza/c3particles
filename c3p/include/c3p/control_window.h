#include <gtk/gtk.h>

typedef struct ControlData
{
  double zoom_scale;
  double pan_scale;

  int g_checkbtn;  // gravity
  double g_scale;
  int s_checkbtn;  // spring
  double s_scale;
  int g_center_checkbtn;
} ControlData;

static void initControls(ControlData* ctl)
{
  ctl->zoom_scale = -100.0;
  ctl->pan_scale = 0;
  ctl->g_checkbtn = 1;
  ctl->g_scale = 4;
  ctl->s_checkbtn = 0;
  ctl->s_scale = 4;
  ctl->g_center_checkbtn = 1;
}

static void print_hello(GtkWidget* widget, gpointer data)
{
  g_print("Hello World\n");
}

static void updateScale(GtkRange* scale, gpointer ctl)
{
  *(double*)ctl = gtk_range_get_value(scale);
}

static void updateCheckbtn(GtkToggleButton* btn, gpointer ctl)
{
  *(int*)ctl = gtk_toggle_button_get_active(btn);
}

static void activate(GtkApplication* app, gpointer user_data)
{
  printf("struct pointer (activate): %p \n", user_data);

  GtkWidget* window;
  GtkWidget* grid;
  GtkWidget* boxtl;  // box top left
  GtkWidget* boxtr;  // box top right

  GtkWidget* button;
  GtkWidget* button_box;
  GtkWidget* zoom_scale;
  GtkWidget* pan_scale;

  GtkWidget* g_checkbtn;  // gravity
  GtkWidget* g_scale;
  GtkWidget* s_checkbtn;  // spring
  GtkWidget* s_scale;
  GtkWidget* g_center_checkbtn;

  // create new window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Particle System Controls");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);

  // construct container that will pack the widgets
  grid = gtk_grid_new();
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
  gtk_container_add(GTK_CONTAINER(window), grid);

  boxtl = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
  gtk_box_set_homogeneous(GTK_BOX(boxtl), TRUE);
  gtk_grid_attach(GTK_GRID(grid), boxtl, 0, 0, 50, 10);

  boxtr = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
  gtk_box_set_homogeneous(GTK_BOX(boxtr), TRUE);
  gtk_grid_attach(GTK_GRID(grid), boxtr, 50, 0, 30, 10);

  zoom_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 10, 1);
  double* zoom_ptr = &(((ControlData*)user_data)->zoom_scale);
  g_signal_connect(zoom_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)zoom_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), zoom_scale, TRUE, FALSE, 20);

  pan_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 10, 1);
  double* pan_ptr = &(((ControlData*)user_data)->pan_scale);
  g_signal_connect(pan_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)pan_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), pan_scale, TRUE, FALSE, 20);

  //  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  //  //gtk_container_add(GTK_CONTAINER(window), button_box);
  //  gtk_box_pack_start(GTK_BOX(boxtl), button_box, TRUE, FALSE, 20);
  //
  //  button = gtk_button_new_with_label ("Hello World");
  //  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  //  gtk_container_add (GTK_CONTAINER(button_box), button);

  g_checkbtn = gtk_check_button_new_with_label("gravity, G = 1 * 10^(-x)");
  int* gcheck_ptr = &(((ControlData*)user_data)->g_checkbtn);
  g_signal_connect(g_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)gcheck_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), g_checkbtn, TRUE, FALSE, 20);

  g_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1, 10, 1);
  double* gscale_ptr = &(((ControlData*)user_data)->g_scale);
  g_signal_connect(g_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)gscale_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), g_scale, TRUE, FALSE, 20);

  s_checkbtn = gtk_check_button_new_with_label("springs");
  int* scheck_ptr = &(((ControlData*)user_data)->s_checkbtn);
  g_signal_connect(s_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)scheck_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), s_checkbtn, TRUE, FALSE, 20);

  s_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1, 10, 1);
  double* sscale_ptr = &(((ControlData*)user_data)->s_scale);
  g_signal_connect(s_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)sscale_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), s_scale, TRUE, FALSE, 20);

  g_center_checkbtn = gtk_check_button_new_with_label("set mass in center");
  int* centercheck_ptr = &(((ControlData*)user_data)->g_center_checkbtn);
  g_signal_connect(g_center_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)centercheck_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), g_center_checkbtn, TRUE, FALSE, 20);

  gtk_widget_show_all(window);
}
