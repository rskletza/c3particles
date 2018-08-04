#ifndef C3P__CONTROL_WINDOW__INCLUDED
#define C3P__CONTROL_WINDOW__INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <gtk/gtk.h>

typedef struct CameraControlData
{
  double dolly_scale;
  double pedestal_scale;
  double truck_scale;

  double pan_scale;
  double tilt_scale;

  double zoom_scale;
} CameraControlData;

typedef struct ParticleControlData
{
  int g_checkbtn;  // gravity
  double g_scale;
  int s_checkbtn;  // spring
  double s_scale;
  int g_center_checkbtn;
  int center_dropdown;

  int trail_checkbtn;
  int restart_btn;
  int reverse_btn;
  int pause_btn;
} ParticleControlData;

static void initCameraControls(CameraControlData* ctl)
{
  ctl->dolly_scale = -100.0;
  ctl->pan_scale = 0.0;
  ctl->tilt_scale = 0.0;
}

static void initParticleControls(ParticleControlData* ctl)
{
  ctl->g_checkbtn = 1;
  ctl->g_scale = -4;
  ctl->s_checkbtn = 0;
  ctl->s_scale = 0.5;
  ctl->g_center_checkbtn = 1;
  ctl->center_dropdown = 0;
  ctl->trail_checkbtn = 0;
  ctl->restart_btn = 0;
  ctl->reverse_btn = 0;
  ctl->pause_btn = 0;
}

static void clickBtn(GtkButton* btn, gpointer ctl) { *(int*)ctl = 1; }

static void updateBtn(GtkButton* btn, gpointer ctl) { *(int*)ctl ^= 1; }

static void updateScale(GtkRange* scale, gpointer ctl)
{
  *(double*)ctl = gtk_range_get_value(scale);
}

static void dropdownChange(GtkComboBox *dd, gpointer ctl)
{
  *(int*)ctl = gtk_combo_box_get_active(dd);
}

static void updateCheckbtn(GtkToggleButton* btn, gpointer ctl)
{
  *(int*)ctl = gtk_toggle_button_get_active(btn);
}

static void createCameraCtlWindow(GtkApplication* app, gpointer user_data)
{
  GtkWidget* window;

  GtkWidget* grid;
  GtkWidget* boxtl;
  GtkWidget* boxtr;

  GtkWidget* dolly_scale;
  GtkWidget* pedestal_scale;
  GtkWidget* truck_scale;

  GtkWidget* pan_scale;
  GtkWidget* tilt_scale;

  GtkWidget* zoom_scale;

  boxtl = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
  gtk_box_set_homogeneous(GTK_BOX(boxtl), TRUE);
  gtk_grid_attach(GTK_GRID(grid), boxtl, 0, 0, 50, 10);

  boxtr = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
  gtk_box_set_homogeneous(GTK_BOX(boxtr), TRUE);
  gtk_grid_attach(GTK_GRID(grid), boxtr, 50, 0, 30, 10);


  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Camera Controls");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);

  grid = gtk_grid_new();
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
  gtk_container_add(GTK_CONTAINER(window), grid);

  dolly_scale =
      gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -100, 0, 10);
  gtk_range_set_value(GTK_RANGE(dolly_scale),
                      ((CameraControlData*)user_data)->dolly_scale);
  double* dolly_ptr = &(((CameraControlData*)user_data)->dolly_scale);
  g_signal_connect(dolly_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)dolly_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), dolly_scale, TRUE, FALSE, 20);

  pan_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -50, 50, 10);
  gtk_range_set_value(GTK_RANGE(pan_scale),
                      ((CameraControlData*)user_data)->pan_scale);
  double* pan_ptr = &(((CameraControlData*)user_data)->pan_scale);
  g_signal_connect(pan_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)pan_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), pan_scale, TRUE, FALSE, 20);

  tilt_scale = gtk_scale_new_with_range(GTK_ORIENTATION_VERTICAL, -50, 50, 10);
  gtk_range_set_value(GTK_RANGE(tilt_scale),
                      ((CameraControlData*)user_data)->tilt_scale);
  double* tilt_ptr = &(((CameraControlData*)user_data)->tilt_scale);
  g_signal_connect(tilt_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)tilt_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), tilt_scale, TRUE, FALSE, 20);
}

static void createParticleCtlWindow(GtkApplication* app, gpointer user_data)
{
  GtkWidget* window;
  GtkWidget* grid;
  GtkWidget* boxtl;  // box top left
  GtkWidget* boxtr;  // box top right

  GtkWidget* rev_button;
  GtkWidget* button_box;

  GtkWidget* g_checkbtn;  // gravity
  GtkWidget* g_scale;
  GtkWidget* s_checkbtn;  // spring
  GtkWidget* s_scale;
  GtkWidget* g_center_checkbtn;
  GtkWidget* center_dropdown;

  GtkWidget* trail_checkbtn;  // trail behind particles
  GtkWidget* restart_btn;
  GtkWidget* reverse_btn;
  GtkWidget* pause_btn;

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

  trail_checkbtn = gtk_check_button_new_with_label("trails");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(trail_checkbtn), FALSE);
  int* trailcheck_ptr = &(((ParticleControlData*)user_data)->trail_checkbtn);
  g_signal_connect(trail_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)trailcheck_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), trail_checkbtn, TRUE, FALSE, 20);

  restart_btn = gtk_button_new_with_label("Restart");
  int* restart_ptr = &(((ParticleControlData*)user_data)->restart_btn);
  g_signal_connect(restart_btn, "clicked", G_CALLBACK(clickBtn),
                   (gpointer)restart_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), restart_btn, TRUE, FALSE, 20);

  g_checkbtn = gtk_check_button_new_with_label("gravity, G = 1 * 10^(x)");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_checkbtn), TRUE);
  int* gcheck_ptr = &(((ParticleControlData*)user_data)->g_checkbtn);
  g_signal_connect(g_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)gcheck_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), g_checkbtn, TRUE, FALSE, 20);

  g_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -10, -1, 1);
  gtk_range_set_value(GTK_RANGE(g_scale), ((ParticleControlData*)user_data)->g_scale);
  double* gscale_ptr = &(((ParticleControlData*)user_data)->g_scale);
  g_signal_connect(g_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)gscale_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), g_scale, TRUE, FALSE, 20);

  s_checkbtn = gtk_check_button_new_with_label("springs");
  int* scheck_ptr = &(((ParticleControlData*)user_data)->s_checkbtn);
  g_signal_connect(s_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)scheck_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), s_checkbtn, TRUE, FALSE, 20);

  s_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.1, 1, 0.1);
  gtk_range_set_value(GTK_RANGE(s_scale), ((ParticleControlData*)user_data)->s_scale);
  double* sscale_ptr = &(((ParticleControlData*)user_data)->s_scale);
  g_signal_connect(s_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)sscale_ptr);
  gtk_box_pack_start(GTK_BOX(boxtl), s_scale, TRUE, FALSE, 20);

//  g_center_checkbtn = gtk_check_button_new_with_label("set mass in center");
//  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_center_checkbtn), TRUE);
//  int* centercheck_ptr = &(((ParticleControlData*)user_data)->g_center_checkbtn);
//  g_signal_connect(g_center_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
//                   (gpointer)centercheck_ptr);
//  gtk_box_pack_start(GTK_BOX(boxtr), g_center_checkbtn, TRUE, FALSE, 20);
  center_dropdown = gtk_combo_box_text_new();
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(center_dropdown), "0", "none");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(center_dropdown), "1", "mass");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(center_dropdown), "2", "spring");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(center_dropdown), "3", "attractor");
  int* centerdd_ptr = &(((ParticleControlData*)user_data)->center_dropdown);
  gtk_combo_box_set_active(GTK_COMBO_BOX(center_dropdown), ((ParticleControlData*)user_data)->center_dropdown);
  g_signal_connect(center_dropdown, "changed", G_CALLBACK(dropdownChange),
                   (gpointer)centerdd_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), center_dropdown, TRUE, FALSE, 20);

  reverse_btn = gtk_button_new_with_label("Reverse Velocity");
  int* reverse_ptr = &(((ParticleControlData*)user_data)->reverse_btn);
  g_signal_connect(reverse_btn, "clicked", G_CALLBACK(clickBtn),
                   (gpointer)reverse_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), reverse_btn, TRUE, FALSE, 20);

  pause_btn = gtk_button_new_with_label("Pause/Resume");
  int* pause_ptr = &(((ParticleControlData*)user_data)->pause_btn);
  g_signal_connect(pause_btn, "clicked", G_CALLBACK(updateBtn),
                   (gpointer)pause_ptr);
  gtk_box_pack_start(GTK_BOX(boxtr), pause_btn, TRUE, FALSE, 20);

  gtk_widget_show_all(window);
}

static void activate(GtkApplication* app, gpointer user_data)
{
  createParticleCtlWindow(app, user_data);
}

#ifdef __cplusplus
}
#endif

#endif //C3P__CONTROL_WINDOW__INCLUDED
