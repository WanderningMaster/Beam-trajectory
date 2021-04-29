#include <gtk/gtk.h>
#include <stdlib.h>
#include <math.h>
#include "Trajectory.h"
#include <string.h>


GtkWidget *window, /*main window*/
      *count, *c, *c1, //entries
      *count_label, *c_label, *c1_label, *results, *help,/*labels*/ 
      *dialog, *content_area, *drawing_area,/*dialog widgets*/
      *v_label_box, *v_entry_box, *hbox,/*box widgets*/
      *button, *zoom;/*submit button*/
     

struct {
  int count;
  double coordx[2];
  double coordy[2];
} draw_point;

struct data{
    struct Point begin;
    struct Point end;
    struct Point *drops;
    double c;
    double c1;
    int count;
};


void on_submit_clicked(GtkWidget *widget, gpointer data);
void on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void on_darea_click_event(GtkWidget *widget, GdkEventButton *event,gpointer user_data);
void draw(cairo_t *cr, struct data my_data);

int main(int argc, char **argv){

    gtk_init(&argc, &argv);

    /*window configuration*/
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Beam trajectory");
    gtk_widget_set_size_request(window, 350,150);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    /*quit event*/
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    

    /*entries*/
    c = gtk_entry_new();
    c1 = gtk_entry_new();
    count = gtk_entry_new();


    /*labels*/
    c_label = gtk_label_new("Lower border: ");
    c1_label = gtk_label_new("Higher border: ");
    count_label = gtk_label_new("Number of beams: ");

    /*form submission event*/
    button = gtk_button_new_with_label("Submit");
    g_signal_connect(button, "clicked", G_CALLBACK(on_submit_clicked), NULL);


    /*packing labels into vertical box*/
    v_label_box = gtk_vbox_new(0,2);
    gtk_box_pack_start(GTK_BOX(v_label_box), c_label, 0,0,6.5);
    gtk_box_pack_start(GTK_BOX(v_label_box), c1_label, 0,0,6.5);
    gtk_box_pack_start(GTK_BOX(v_label_box), count_label, 0,0,6.5);
    
    /*packing entries and submit button into vertical box*/
    v_entry_box = gtk_vbox_new(1,0);
    gtk_box_pack_start(GTK_BOX(v_entry_box), c, 0,0,0);
    gtk_box_pack_start(GTK_BOX(v_entry_box), c1, 0,0,0);
    gtk_box_pack_start(GTK_BOX(v_entry_box), count, 0,0,0);
    gtk_box_pack_start(GTK_BOX(v_entry_box), button, 0,0,0);

    /*packing boxes into horizontal box*/
    hbox = gtk_hbox_new(1 ,0);
    gtk_box_pack_start(GTK_BOX(hbox), v_label_box, 0,0,0);
    gtk_box_pack_start(GTK_BOX(hbox), v_entry_box, 0,0,0);

    gtk_container_add(GTK_CONTAINER(window), hbox);

    gtk_widget_show_all(window);
    gtk_main();
}

void on_submit_clicked(GtkWidget *widget, gpointer data){

    drawing_area = gtk_drawing_area_new ();
    results = gtk_label_new("Result");
    help = gtk_label_new("Select the start and end point of the trajectory with the mouse");

    /*dialog configuration*/
    dialog = gtk_message_dialog_new
    (
                        GTK_WINDOW(window),
                        GTK_DIALOG_DESTROY_WITH_PARENT,
                        GTK_MESSAGE_INFO,
                        GTK_BUTTONS_OK,
                        NULL
    );
    gtk_window_set_title(GTK_WINDOW(dialog), "Trajectory");
    gtk_widget_add_events(GTK_WINDOW(drawing_area), GDK_BUTTON_PRESS_MASK);
    g_signal_connect(GTK_WINDOW(drawing_area), "button-press-event", 
      G_CALLBACK(on_darea_click_event), NULL);   


    /*connect content area with dialog*/
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog)); 


    /*pack drawing area and label into content area*/
    gtk_container_add(GTK_CONTAINER(content_area), drawing_area);
    gtk_container_add(GTK_CONTAINER(content_area), results);
    gtk_container_add(GTK_CONTAINER(content_area), help);

    /*drawing area configuration*/
    double _c = atof((char*)gtk_entry_get_text(GTK_ENTRY(c)));
    double _c1 = atof((char*)gtk_entry_get_text(GTK_ENTRY(c1)));
    gtk_widget_set_size_request (drawing_area, 800, 800);

    //background color
    GdkColor color;
    gdk_color_parse ("white", &color);
    gtk_widget_modify_bg ( GTK_WIDGET(drawing_area), GTK_STATE_NORMAL, &color);



    
    /*When start drawing event*/
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL); 


    //showing our widgets
    gtk_widget_show(drawing_area);
    gtk_widget_show(results);
    gtk_widget_show(help);

    /*Run dialog*/
    gtk_dialog_run(GTK_DIALOG(dialog));

    /*Destroy it on quit*/
    gtk_widget_destroy(dialog);
}

void on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data){
    double _c = atof((char*)gtk_entry_get_text(GTK_ENTRY(c)));
    double _c1 = atof((char*)gtk_entry_get_text(GTK_ENTRY(c1)));
    double _begin_x = draw_point.coordx[0]-400;
    double _begin_y = 400-draw_point.coordy[0];
    double _end_x = draw_point.coordx[1]-400;
    double _end_y = 400-draw_point.coordy[1];
    int _count = atoi((char*)gtk_entry_get_text(GTK_ENTRY(count)));
    double length; 
    double corner;

   // printf("begin(%lf, %lf), end(%lf, %lf)\n", _begin_x, _begin_y, _end_x, _end_y);
    
    struct Point begin;
    begin.x = _begin_x;
    begin.y = _begin_y;

    struct Point end;
    end.x = _end_x;
    end.y = _end_y;
  
    draw_point.count = 0;

    struct Point *drops;
    drops = (struct Point*)malloc(sizeof(struct Point)*(_count-1));
    get_drop_points(begin, end, drops, _c, _c1, _count);

    struct data my_data = {.begin = begin, .end = end, .drops = drops, .c = _c, .c1 = _c1, .count = _count};
    gtk_label_set_text(results, "buf");
    draw(cr, my_data);
    char *buf;
    length = get_trajectory_length(begin, end, drops, _count);
    corner = get_corner(begin, end, drops, _count);
    asprintf(&buf, "Begin corner: %lf°, End corner %lf°, Length: %lf\nbegin(%lf, %lf), end(%lf, %lf)",corner, corner, 
                                                                                                    length, _begin_x, _begin_y, _end_x, _end_y);
    gtk_label_set_text(results, buf); 
    
    for(int i = 0; i<2;i++){
        draw_point.coordx[i] = 0;
        draw_point.coordy[i] = 0;
    }
    free(drops);
}

void draw(cairo_t *cr, struct data my_data){

    double _begin_x = my_data.begin.x;
    double _begin_y = my_data.begin.y;
    struct Point *drops = my_data.drops;
    double _end_x = my_data.end.x;
    double _end_y = my_data.end.y;
    double _c = my_data.c;
    double _c1 = my_data.c1;
    int _count = my_data.count;   

    cairo_translate(cr, 400, 400);

    cairo_set_source_rgb(cr, 0,0,0);
    cairo_set_line_width(cr, 1.0);

    cairo_move_to(cr, -400, (-1)*_c);
    cairo_line_to(cr, 400, (-1)*_c);

    cairo_move_to(cr, -400, (-1)*_c1);
    cairo_line_to(cr, 400, (-1)*_c1);

    cairo_move_to(cr, _begin_x, _begin_y*(-1));
    for(int i = 0; i<_count-1; i++){

        cairo_line_to(cr, drops[i].x, drops[i].y*(-1));
    }
    cairo_line_to(cr, _end_x, _end_y*(-1));


    cairo_stroke(cr);
       
}  


void on_darea_click_event(GtkWidget *widget, GdkEventButton *event,gpointer user_data){
    if(draw_point.count == 1) {
      gtk_widget_queue_draw(widget);
    }
    if (event->button == 1) {
        draw_point.coordx[draw_point.count] = event->x;
        draw_point.coordy[draw_point.count] = event->y;
        draw_point.count++;
    }
}