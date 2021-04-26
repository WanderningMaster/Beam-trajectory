#include <gtk/gtk.h>

GtkWidget *window, /*main window*/
      *count, *c, *c1, *begin_x, *begin_y, *end_x, *end_y, //entries
      *count_label, *c_label, *c1_label, *begin_x_label, *begin_y_label, *end_x_label, *end_y_label, *results, /*labels*/ 
      *dialog, *content_area, *drawing_area,/*dialog widgets*/
      *v_label_box, *v_entry_box, *hbox,/*box widgets*/
      *button;/*submit button*/

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
    begin_x = gtk_entry_new();
    begin_y = gtk_entry_new();
    end_x = gtk_entry_new();
    end_y = gtk_entry_new();
    c = gtk_entry_new();
    c1 = gtk_entry_new();
    count = gtk_entry_new();


    /*labels*/
    begin_x_label = gtk_label_new("Begin x coordinate: ");
    begin_y_label = gtk_label_new("Begin y coordinate: ");
    end_x_label = gtk_label_new("End x coordinate: ");
    end_y_label = gtk_label_new("End y coordinate: ");
    c_label = gtk_label_new("Lower border: ");
    c1_label = gtk_label_new("Higher border: ");
    count_label = gtk_label_new("Number of beams: ");

    /*form submission event*/
    button = gtk_button_new_with_label("Submit");
    //g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);


    /*packing labels into vertical box*/
    v_label_box = gtk_vbox_new(0,2);
    gtk_box_pack_start(GTK_BOX(v_label_box), begin_x_label, 0,0,6.5);
    gtk_box_pack_start(GTK_BOX(v_label_box), begin_y_label, 0,0,6.5);
    gtk_box_pack_start(GTK_BOX(v_label_box), end_x_label, 0,0,6.5);
    gtk_box_pack_start(GTK_BOX(v_label_box), end_y_label, 0,0,6.5);
    gtk_box_pack_start(GTK_BOX(v_label_box), c_label, 0,0,6.5);
    gtk_box_pack_start(GTK_BOX(v_label_box), c1_label, 0,0,6.5);
    gtk_box_pack_start(GTK_BOX(v_label_box), count_label, 0,0,6.5);
    
    /*packing entries and submit button into vertical box*/
    v_entry_box = gtk_vbox_new(1,0);
    gtk_box_pack_start(GTK_BOX(v_entry_box), begin_x, 0,0,0);
    gtk_box_pack_start(GTK_BOX(v_entry_box), begin_y, 0,0,0);
    gtk_box_pack_start(GTK_BOX(v_entry_box), end_x, 0,0,0);
    gtk_box_pack_start(GTK_BOX(v_entry_box), end_y, 0,0,0);
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