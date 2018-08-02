#include <gtk/gtk.h>

typedef struct ControlData
{
  double dolly_scale;
  double pan_scale;
  double tilt_scale;

  int g_checkbtn;  // gravity
  double g_scale;
  int s_checkbtn;  // spring
  double s_scale;
  int g_center_checkbtn;

  int trail_checkbtn;
  int restart_btn;
  int reverse_btn;
} ControlData;

static void initControls(ControlData* ctl)
{
  ctl->dolly_scale = -100.0;
  ctl->pan_scale = 0.0;
  ctl->tilt_scale = 0.0;
  ctl->g_checkbtn = 1;
  ctl->g_scale = -4;
  ctl->s_checkbtn = 0;
  ctl->s_scale = 4;
  ctl->g_center_checkbtn = 1;
  ctl->trail_checkbtn = 0;
  ctl->restart_btn = 0;
  ctl->reverse_btn = 0;
}

static void updateBtn(GtkButton* btn, gpointer ctl) { *(int*)ctl = 1; }
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

  GtkWidget* rev_button;
  GtkWidget* button_box;
  GtkWidget* dolly_scale;
  GtkWidget* pan_scale;
  GtkWidget* tilt_scale;

  GtkWidget* g_checkbtn;  // gravity
  GtkWidget* g_scale;
  GtkWidget* s_checkbtn;  // spring
  GtkWidget* s_scale;
  GtkWidget* g_center_checkbtn;

  GtkWidget* trail_checkbtn;  // trail behind particles
  GtkWidget* restart_btn;
  GtkWidget* reverse_btn;

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

  dolly_scale =
      gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -100, 0, 10);
  gtk_range_set_value(GTK_RANGE(dolly_scale),
                      ((ControlData*)user_data)->dolly_scale);
  double* dolly_ptr = &(((ControlData*)user_data)->dolly_scale);
  g_signal_connect(dolly_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)dolly_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), dolly_scale, TRUE, FALSE, 20);

  pan_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -50, 50, 10);
  gtk_range_set_value(GTK_RANGE(pan_scale),
                      ((ControlData*)user_data)->pan_scale);
  double* pan_ptr = &(((ControlData*)user_data)->pan_scale);
  g_signal_connect(pan_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)pan_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), pan_scale, TRUE, FALSE, 20);

  tilt_scale = gtk_scale_new_with_range(GTK_ORIENTATION_VERTICAL, -50, 50, 10);
  gtk_range_set_value(GTK_RANGE(tilt_scale),
                      ((ControlData*)user_data)->tilt_scale);
  double* tilt_ptr = &(((ControlData*)user_data)->tilt_scale);
  g_signal_connect(tilt_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)tilt_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), tilt_scale, TRUE, FALSE, 20);

  trail_checkbtn = gtk_check_button_new_with_label("trails");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(trail_checkbtn), FALSE);
  int* trailcheck_ptr = &(((ControlData*)user_data)->trail_checkbtn);
  g_signal_connect(trail_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)trailcheck_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), trail_checkbtn, TRUE, FALSE, 20);

  restart_btn = gtk_button_new_with_label("Restart");
  int* restart_ptr = &(((ControlData*)user_data)->restart_btn);
  g_signal_connect(restart_btn, "clicked", G_CALLBACK(updateBtn),
                   (gpointer)restart_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), restart_btn, TRUE, FALSE, 20);

  g_checkbtn = gtk_check_button_new_with_label("gravity, G = 1 * 10^(x)");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_checkbtn), TRUE);
  int* gcheck_ptr = &(((ControlData*)user_data)->g_checkbtn);
  g_signal_connect(g_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)gcheck_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), g_checkbtn, TRUE, FALSE, 20);

  g_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -10, -1, 1);
  gtk_range_set_value(GTK_RANGE(g_scale), ((ControlData*)user_data)->g_scale);
  double* gscale_ptr = &(((ControlData*)user_data)->g_scale);
  g_signal_connect(g_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)gscale_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), g_scale, TRUE, FALSE, 20);

  //s_checkbtn = gtk_check_button_new_with_label("springs");
  //int* scheck_ptr = &(((ControlData*)user_data)->s_checkbtn);
  //g_signal_connect(s_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
  //                 (gpointer)scheck_ptr);
  //gtk_box_pack_start(GTK_BOX(boxtr), s_checkbtn, TRUE, FALSE, 20);

  //s_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1, 10, 1);
  //double* sscale_ptr = &(((ControlData*)user_data)->s_scale);
  //g_signal_connect(s_scale, "value-changed", G_CALLBACK(updateScale),
  //                 (gpointer)sscale_ptr);
  //gtk_box_pack_start(GTK_BOX(boxtr), s_scale, TRUE, FALSE, 20);

  g_center_checkbtn = gtk_check_button_new_with_label("set mass in center");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_center_checkbtn), TRUE);
  int* centercheck_ptr = &(((ControlData*)user_data)->g_center_checkbtn);
  g_signal_connect(g_center_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)centercheck_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), g_center_checkbtn, TRUE, FALSE, 20);

  reverse_btn = gtk_button_new_with_label("Reverse Velocity");
  int* reverse_ptr = &(((ControlData*)user_data)->reverse_btn);
  g_signal_connect(reverse_btn, "clicked", G_CALLBACK(updateBtn),
                   (gpointer)reverse_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), reverse_btn, TRUE, FALSE, 20);

  gtk_widget_show_all(window);
}
