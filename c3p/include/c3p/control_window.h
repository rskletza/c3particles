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
  double sl_scale;  // spring length

  int c_gravity_checkbtn;
  int c_spring_checkbtn;
  int c_attract_checkbtn;
  int center_dropdown;

  int cage_checkbtn;
  int trail_checkbtn;
  int restart_btn;
  int reverse_btn;
  int pause_btn;

  double num_particles;
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
  ctl->sl_scale = 15;  // spring length

  ctl->c_gravity_checkbtn = 0;
  ctl->c_spring_checkbtn = 0;
  ctl->c_attract_checkbtn = 0;

  ctl->trail_checkbtn = 0;
  ctl->restart_btn = 0;
  ctl->reverse_btn = 0;
  ctl->pause_btn = 0;
  ctl->cage_checkbtn = 0;

  ctl->num_particles = 100;
}

static void clickBtn(GtkButton* btn, gpointer ctl) { *(int*)ctl = 1; }
static void updateBtn(GtkButton* btn, gpointer ctl) { *(int*)ctl ^= 1; }
static void updateScale(GtkRange* scale, gpointer ctl)
{
  *(double*)ctl = gtk_range_get_value(scale);
}

static void dropdownChange(GtkComboBox* dd, gpointer ctl)
{
  *(int*)ctl = gtk_combo_box_get_active(dd);
}

static void updateCheckbtn(GtkToggleButton* btn, gpointer ctl)
{
  *(int*)ctl = gtk_toggle_button_get_active(btn);
}

static void createParticleCtlWindow(GtkApplication* app, gpointer user_data)
{
  GtkWidget* window;
  GtkWidget* grid;
  GtkWidget* bigbox;  // box top left
  GtkWidget* box0;
  GtkWidget* box1;
  GtkWidget* box2;
  GtkWidget* box3;
  GtkWidget* box4;
  GtkWidget* box5;
  GtkWidget* box6;
  GtkWidget* box7;
  GtkWidget* box8;
  GtkWidget* box9;

  GtkWidget* rev_button;
  GtkWidget* button_box;

  GtkWidget* g_checkbtn;  // gravity
  GtkWidget* g_scale;
  GtkWidget* s_checkbtn;  // spring
  GtkWidget* s_scale;
  GtkWidget* sl_scale;

  GtkWidget* c_gravity_checkbtn;
  GtkWidget* c_spring_checkbtn;
  GtkWidget* c_attract_checkbtn;

  GtkWidget* cage_checkbtn;
  GtkWidget* trail_checkbtn;  // trail behind particles
  GtkWidget* restart_btn;
  GtkWidget* reverse_btn;
  GtkWidget* pause_btn;

  GtkWidget* spring_c;
  GtkWidget* gravity_c;

  GtkWidget* num_particles;

  GtkWidget* num_particles_label;

  // create new window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Particle System Controls");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);

  // construct container that will pack the widgets
  //  grid = gtk_grid_new();
  //  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
  //  gtk_container_add(GTK_CONTAINER(window), grid);

  bigbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  //  gtk_box_set_homogeneous(GTK_BOX(bigbox), TRUE);
  gtk_container_add(GTK_CONTAINER(window), bigbox);

  box0 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box0), TRUE);
  gtk_box_pack_start(GTK_BOX(bigbox), box0, TRUE, FALSE, 20);

  num_particles_label = gtk_label_new("particle number");
  gtk_box_pack_start(GTK_BOX(box0), num_particles_label, TRUE, FALSE, 5);
  box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box1), TRUE);
  gtk_box_pack_start(GTK_BOX(bigbox), box1, TRUE, FALSE, 20);

  box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box2), TRUE);
  //  gtk_box_pack_start(GTK_BOX(bigbox), box2, TRUE, FALSE, 20);

  box3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box3), TRUE);
  gtk_box_pack_start(GTK_BOX(bigbox), box3, TRUE, FALSE, 20);

  gravity_c = gtk_label_new("gravity exponent");
  gtk_box_pack_start(GTK_BOX(box1), gravity_c, TRUE, FALSE, 0);
  box4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box4), TRUE);
  // gtk_box_pack_start(GTK_BOX(bigbox), box4, TRUE, FALSE, 20);

  box5 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box5), TRUE);
  gtk_box_pack_start(GTK_BOX(bigbox), box5, TRUE, FALSE, 20);

  box6 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box6), TRUE);
  gtk_box_pack_start(GTK_BOX(bigbox), box6, TRUE, FALSE, 20);

  box7 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box7), TRUE);
  gtk_box_pack_start(GTK_BOX(bigbox), box7, TRUE, FALSE, 20);

  box8 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box8), TRUE);
  // gtk_box_pack_start(GTK_BOX(bigbox), box8, TRUE, FALSE, 20);

  box9 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box9), TRUE);
  gtk_box_pack_start(GTK_BOX(bigbox), box9, TRUE, FALSE, 20);

  spring_c = gtk_label_new("spring constant, spring length");
  gtk_box_pack_start(GTK_BOX(box3), spring_c, TRUE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box3), box3, TRUE, FALSE, 20);
  //  gravity_c;

  c_spring_checkbtn = gtk_check_button_new_with_label("center spring");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(c_spring_checkbtn), FALSE);
  int* c_springcheck_ptr =
      &(((ParticleControlData*)user_data)->c_spring_checkbtn);
  g_signal_connect(c_spring_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)c_springcheck_ptr);
  gtk_box_pack_start(GTK_BOX(box6), c_spring_checkbtn, TRUE, FALSE, 20);

  c_gravity_checkbtn = gtk_check_button_new_with_label("center mass");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(c_gravity_checkbtn), FALSE);
  int* c_gravitycheck_ptr =
      &(((ParticleControlData*)user_data)->c_gravity_checkbtn);
  g_signal_connect(c_gravity_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)c_gravitycheck_ptr);
  gtk_box_pack_start(GTK_BOX(box6), c_gravity_checkbtn, TRUE, FALSE, 20);

  c_attract_checkbtn = gtk_check_button_new_with_label("center attract");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(c_attract_checkbtn), FALSE);
  int* c_attractcheck_ptr =
      &(((ParticleControlData*)user_data)->c_attract_checkbtn);
  g_signal_connect(c_attract_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)c_attractcheck_ptr);
  gtk_box_pack_start(GTK_BOX(box6), c_attract_checkbtn, TRUE, FALSE, 20);

  trail_checkbtn = gtk_check_button_new_with_label("trails");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(trail_checkbtn), FALSE);
  int* trailcheck_ptr = &(((ParticleControlData*)user_data)->trail_checkbtn);
  g_signal_connect(trail_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)trailcheck_ptr);
  gtk_box_pack_start(GTK_BOX(box7), trail_checkbtn, TRUE, FALSE, 20);

  cage_checkbtn = gtk_check_button_new_with_label("cage");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cage_checkbtn), FALSE);
  int* cagecheck_ptr = &(((ParticleControlData*)user_data)->cage_checkbtn);
  g_signal_connect(cage_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)cagecheck_ptr);
  gtk_box_pack_start(GTK_BOX(box7), cage_checkbtn, TRUE, FALSE, 20);

  restart_btn = gtk_button_new_with_label("Restart");
  int* restart_ptr = &(((ParticleControlData*)user_data)->restart_btn);
  g_signal_connect(restart_btn, "clicked", G_CALLBACK(clickBtn),
                   (gpointer)restart_ptr);
  gtk_box_pack_start(GTK_BOX(box9), restart_btn, TRUE, FALSE, 20);

  g_checkbtn = gtk_check_button_new_with_label("NxN gravity");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_checkbtn), TRUE);
  int* gcheck_ptr = &(((ParticleControlData*)user_data)->g_checkbtn);
  g_signal_connect(g_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)gcheck_ptr);
  gtk_box_pack_start(GTK_BOX(box5), g_checkbtn, TRUE, FALSE, 20);

  g_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -10, -1, 1);
  g_object_set(g_scale, "width-request", 200, NULL),
      gtk_range_set_value(GTK_RANGE(g_scale),
                          ((ParticleControlData*)user_data)->g_scale);
  double* gscale_ptr = &(((ParticleControlData*)user_data)->g_scale);
  g_signal_connect(g_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)gscale_ptr);
  gtk_box_pack_start(GTK_BOX(box1), g_scale, TRUE, FALSE, 20);

  s_checkbtn = gtk_check_button_new_with_label("NxN springs");
  int* scheck_ptr = &(((ParticleControlData*)user_data)->s_checkbtn);
  g_signal_connect(s_checkbtn, "toggled", G_CALLBACK(updateCheckbtn),
                   (gpointer)scheck_ptr);
  gtk_box_pack_start(GTK_BOX(box5), s_checkbtn, TRUE, FALSE, 20);

  s_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.1, 1, 0.1);
  g_object_set(s_scale, "width-request", 200, NULL),
      gtk_range_set_value(GTK_RANGE(s_scale),
                          ((ParticleControlData*)user_data)->s_scale);
  double* sscale_ptr = &(((ParticleControlData*)user_data)->s_scale);
  g_signal_connect(s_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)sscale_ptr);
  gtk_box_pack_start(GTK_BOX(box3), s_scale, TRUE, FALSE, 20);

  sl_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 10, 20, 1);
  g_object_set(sl_scale, "width-request", 200, NULL),
      gtk_range_set_value(GTK_RANGE(s_scale),
                          ((ParticleControlData*)user_data)->sl_scale);
  double* slscale_ptr = &(((ParticleControlData*)user_data)->sl_scale);
  g_signal_connect(sl_scale, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)slscale_ptr);
  gtk_box_pack_start(GTK_BOX(box3), sl_scale, TRUE, FALSE, 20);

  //  center_dropdown = gtk_combo_box_text_new();
  //  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(center_dropdown), "0",
  //  "none");
  //  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(center_dropdown), "1",
  //  "mass");
  //  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(center_dropdown), "2",
  //  "spring");
  //  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(center_dropdown), "3",
  //  "attractor");
  //  int* centerdd_ptr = &(((ParticleControlData*)user_data)->center_dropdown);
  //  gtk_combo_box_set_active(GTK_COMBO_BOX(center_dropdown),
  //  ((ParticleControlData*)user_data)->center_dropdown);
  //  g_signal_connect(center_dropdown, "changed", G_CALLBACK(dropdownChange),
  //                   (gpointer)centerdd_ptr);
  //  gtk_box_pack_start(GTK_BOX(boxtr), center_dropdown, TRUE, FALSE, 20);

  reverse_btn = gtk_button_new_with_label("Reverse Velocity");
  int* reverse_ptr = &(((ParticleControlData*)user_data)->reverse_btn);
  g_signal_connect(reverse_btn, "clicked", G_CALLBACK(clickBtn),
                   (gpointer)reverse_ptr);
  gtk_box_pack_start(GTK_BOX(box7), reverse_btn, TRUE, FALSE, 20);

  pause_btn = gtk_button_new_with_label("Pause/Resume");
  int* pause_ptr = &(((ParticleControlData*)user_data)->pause_btn);
  g_signal_connect(pause_btn, "clicked", G_CALLBACK(updateBtn),
                   (gpointer)pause_ptr);
  gtk_box_pack_start(GTK_BOX(box9), pause_btn, TRUE, FALSE, 20);

  num_particles =
      gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1, 201, 10);
  g_object_set(num_particles, "width-request", 200, NULL),
      gtk_range_set_value(GTK_RANGE(num_particles),
                          ((ParticleControlData*)user_data)->num_particles);
  double* partscale_ptr = &(((ParticleControlData*)user_data)->num_particles);
  g_signal_connect(num_particles, "value-changed", G_CALLBACK(updateScale),
                   (gpointer)partscale_ptr);
  gtk_box_pack_start(GTK_BOX(box0), num_particles, TRUE, FALSE, 20);

  gtk_widget_show_all(window);
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

static void activate(GtkApplication* app, gpointer user_data)
{
  createParticleCtlWindow(app, user_data);
}

#ifdef __cplusplus
}
#endif

#endif  // C3P__CONTROL_WINDOW__INCLUDED
