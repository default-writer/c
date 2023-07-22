/*
 *
 *  MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include <gtk/gtk.h>

static void print_hello(GtkWidget* widget, gpointer data) {
    g_print("Hello World\n");
}

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget* window;
    GtkWidget* button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    button = gtk_button_new_with_label("Hello World");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    gtk_window_set_child(GTK_WINDOW(window), button);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char** argv) {
    int status = 0;
    const char* ui_mode = getenv("UI_MODE");
    if (ui_mode != 0 && strcmp(ui_mode, "--ui") == 0) {
        GtkApplication* app;
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        g_application_add_main_option(G_APPLICATION(app), "ui", 0, 0, G_OPTION_ARG_NONE, "Start GUI", NULL);
        g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
        status = g_application_run(G_APPLICATION(app), argc, argv);
        g_object_unref(app);
    }
    return status;
}
