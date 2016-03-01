/**
@file
@author Ben Yuan
@date 2013
@copyright 2-clause BSD; see License section

@brief
The client executable (including GUI).

@section License

Copyright (c) 2012-2013 California Institute of Technology.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the California Institute of Technology.

*/

#include "client2.hpp"

// static function forward declarations
static gboolean PollSocket(gpointer);
static int SendMessage(MESSAGE_TYPE, std::string *);
static void AddLineToBuffer(gpointer);
static void ProcessChatLine(GtkWidget *, gpointer);
static void SetUserList(std::list<std::string> *);
static int DoConnect(const char *, uint16_t, const char *);
static int DoDisconnect(void);
static void ShowConnectDialog(GtkWidget *, gpointer);
static void DisconnectCallback(GtkWidget *, gpointer);
static void destroy(GtkWidget *, gpointer);


// static variables for ourself

static GtkBuilder * gtk_builder;
static GtkTextView * textarea;
static GtkEntry * chatentry;
static GtkListStore * userlist;
static GtkStatusbar * status_bar;

static GtkToolButton * toolbar_connect;
static GtkToolButton * toolbar_close;
static GtkToggleToolButton * toolbar_autoscroll;

static GtkDialog * connect_dialog;
static GtkEntry * dialog_hostname;
static GtkSpinButton * dialog_port;
static GtkEntry * dialog_username;

static bool is_autoscroll;
static bool connected;

static CS2Net::Socket * sock;

// Callbacks galore!

/**
 * @brief Opens a connection to a remote server.
 *
 * @param hostname  the remote hostname to which to connect.
 * @param port      the remote port to which to connect.
 * @param username  the username with which to associate at the remote end
 *
 * @attention This function is student-implemented.
 * This function must do two things:
 * - connect to the remote server
 * - send a username request
 * If it fails on either step, it must abort the connection attempt (and close
 * any open socket) and report an error.
 *
 * @return 0 on success. -1 on error.
 */
static int DoConnect(const char * hostname, uint16_t port, const char * username)
{
    std::string __hostname(hostname);
    std::string __username(username);
    guint ctx = gtk_statusbar_get_context_id(status_bar, "none");
    char status_str[1024];

    /* TODO: Fix this function and make it useful. */

    snprintf(status_str, 1024, "Connecting not implemented");
    gtk_statusbar_pop(status_bar, ctx);
    gtk_statusbar_push(status_bar, ctx, status_str);

    return -1;
}

/**
 * @brief Disconnects an open connection.
 */
static int DoDisconnect()
{
    sock->Disconnect();
    connected = false;
    gtk_widget_set_sensitive(GTK_WIDGET(toolbar_connect), true);
    gtk_widget_set_sensitive(GTK_WIDGET(toolbar_close), false);
}

/**
 * @brief The idle function that polls the socket for incoming data and
 * reacts to it as appropriate.
 *
 * @attention This function is student-implemented.
 * You should return false if the socket gets disconnected,
 * and true otherwise.
 *
 * @param data not used.
 *
 * @return true if this function should be called again at the next idle;
 *          false otherwise.
 *
 */
static gboolean PollSocket(gpointer data)
{
    /* TODO: Fix this function so it does something useful. */
    return false;
}

/**
 * @brief Sends a message to the remote server.
 *
 * Given a message type and a message payload, this function generates a string
 * to send over the network by calling EncodeNetworkMessage(), then sends the
 * message on the currently connected socket.
 *
 * @attention This function is student-implemented.
 * Make sure you handle all the possible error cases.
 *
 * @param type      the type of message to send.
 * @param payload   the actual payload to send.
 *
 * @return the number of characters sent, or -1 on send error,
 * or -2 on programmer usage error.
 *
 */
static int SendMessage(MESSAGE_TYPE type, std::string * payload)
{
    /* TODO: make this function useful. */
    ERROR("sending messages is not implemented yet");
    return -2;
}

/**
 * @brief Processes a line of text entered by the user.
 *
 * @attention This function is student-implemented.
 *
 * @param userdata not used.
 */
static void ProcessChatLine(GtkWidget * w, gpointer data)
{
    // some default behavior goes here
    GtkEntry* __w = (GtkEntry*)w;
    const char * txt = gtk_entry_get_text(__w);

    /*
     * TODO: Add some code here to react to a processed chat line.
     * The chat box will always be used to send chat messages only,
     * so you don't have to worry about trying to do lots of parsing.
     * Just create a MSG_CHATMSG message and send it over
     * the open socket.
     */

    // The default behavior (for demonstration) is to print the chat line to the
    // chat buffer exactly as entered.
    // You may want to alter or remove this behavior as you see fit.
    AddLineToBuffer((gpointer)txt);

    // clear the chat line for the next entry
    gtk_entry_set_text(__w,"");

}

/**
 * @brief Adds a line to the chat window buffer.
 *
 * @param userdata pointer to a null-terminated C string
 *                  representing the data to add.
 */
static void AddLineToBuffer(gpointer userdata)
{
    // grab the buffer corresponding to the global text area
    GtkTextBuffer * buf = gtk_text_view_get_buffer(textarea);
    // set up some temporary constructs (iterator and mark)
    GtkTextIter iter;
    GtkTextMark * m;

    // grab the line to insert
    const char * to_insert = (const char *) userdata;

    // insert the line at the end
    gtk_text_buffer_get_end_iter(buf, &iter);
    gtk_text_buffer_insert(buf, &iter, to_insert, -1);
    gtk_text_buffer_insert(buf, &iter, "\n", -1);

    // autoscroll the buffer forward
    m = gtk_text_buffer_create_mark(buf, NULL, &iter, FALSE);
    gtk_text_view_scroll_to_mark(textarea, m, 0, FALSE, 0, 0);
    gtk_text_buffer_delete_mark(buf, m);
}

/**
 * @brief Sets the user list to the provided list of strings.
 *
 * @param data a list of strings (one per element) to set as the user list.
 */
static void SetUserList(std::list<std::string> * data)
{
    GtkTreeIter treeiter;
    std::list<std::string>::iterator dataiter;

    gtk_list_store_clear(userlist);
    for(dataiter = data->begin(); dataiter != data->end(); dataiter++)
    {
        gtk_list_store_insert_with_values(userlist, &treeiter, -1, 0, (*dataiter).c_str(), -1);
    }
}

/**
 * @brief Displays the connection dialog.
 */
static void ShowConnectDialog(GtkWidget * w, gpointer data)
{

    gint result = gtk_dialog_run(connect_dialog);
    const char * hostname;
    const char * username;
    uint16_t port;
    switch(result)
    {
        case GTK_RESPONSE_ACCEPT:
            // The user pressed CONNECT here.
            // We should therefore process this connection request.
            hostname = gtk_entry_get_text(dialog_hostname);
            port = (uint16_t) gtk_spin_button_get_value_as_int(dialog_port);
            username = gtk_entry_get_text(dialog_username);

            fprintf(stderr, "connecting to %s:%hu as %s\n", hostname, port, username);
            DoConnect(hostname, port, username);

            // if we're connected here then register the poll idle callback
            // and make the close button active
            if(connected)
            {
                g_idle_add(PollSocket, NULL);
                gtk_widget_set_sensitive(GTK_WIDGET(toolbar_connect), false);
                gtk_widget_set_sensitive(GTK_WIDGET(toolbar_close), true);
            }

            break;
        default:
            // The user pressed CANCEL here.
            break;
    }
    gtk_widget_hide(GTK_WIDGET(connect_dialog));

}

/**
 * @brief UI callback for the disconnect button.
 */
static void DisconnectCallback(GtkWidget * w, gpointer data)
{
    guint ctx = gtk_statusbar_get_context_id(status_bar, "none");
    char status_str[1024];
    // disconnect from the server
    DoDisconnect();
    // then push a status string to the status bar
    snprintf(status_str, 1024, "Not connected");
    gtk_statusbar_pop(status_bar, ctx);
    gtk_statusbar_push(status_bar, ctx, status_str);
}

/**
 * @brief Quits the GTK application.
 *
 * This gets called when the user closes the main window (or otherwise quits
 * this application). Its main purpose is to make sure the global socket gets
 * closed properly.
 */
static void destroy(GtkWidget * w, gpointer data)
{
    // delete the global socket (disconnecting if necessary)
    delete sock;
    // then quit
    gtk_main_quit();
}

/**
 * @brief The main application routine.
 *
 * This function sets up all the things, including signals and whatnot,
 * then tells the whole thing to run.
 */
int main(int argc, char ** argv)
{
    GtkWidget * window, * quit_button;

    // create a new socket
    sock = new CS2Net::Socket();
    connected = false;

    // initialise GTK
    gtk_init(&argc, &argv);

    int ret;
    GError * err = NULL;

    gtk_builder = gtk_builder_new();
    ret = gtk_builder_add_from_file(gtk_builder, "./client2.glade", &err);
    if(!ret) // GTK functions usually return 0 on error
    {
        // if we can't load our UI then we can't continue!
        fprintf(stderr, "%s", err->message);
        exit(1);
    }

    // connect some basic signals
    // we connect more signals later
    gtk_builder_connect_signals(gtk_builder, NULL);

    // set up static variables (pointers to UI elements)

    userlist = (GtkListStore *) gtk_builder_get_object(gtk_builder, "user_list_store");
    textarea = (GtkTextView *) gtk_builder_get_object(gtk_builder, "chat_buffer");
    chatentry = (GtkEntry *) gtk_builder_get_object(gtk_builder, "chat_entry_box");
    status_bar = (GtkStatusbar *) gtk_builder_get_object(gtk_builder, "status_bar");

    toolbar_connect = (GtkToolButton *) gtk_builder_get_object(gtk_builder, "connect_button");
    toolbar_close = (GtkToolButton *) gtk_builder_get_object(gtk_builder, "close_button");

    connect_dialog = (GtkDialog *) gtk_builder_get_object(gtk_builder, "connect_dialog");
    dialog_hostname = (GtkEntry *) gtk_builder_get_object(gtk_builder, "dialog_hostname");
    dialog_username = (GtkEntry *) gtk_builder_get_object(gtk_builder, "dialog_username");
    dialog_port = (GtkSpinButton *) gtk_builder_get_object(gtk_builder, "dialog_port");

    // set up local pointers to UI elements
    window = (GtkWidget *) gtk_builder_get_object(gtk_builder, "main_wnd");
    quit_button = (GtkWidget *) gtk_builder_get_object(gtk_builder, "quit_button");

    // initialise some default values

    // connect signals to important UI elements
    g_signal_connect(chatentry, "activate", G_CALLBACK(ProcessChatLine), NULL);
    g_signal_connect(toolbar_connect, "clicked", G_CALLBACK(ShowConnectDialog), NULL);
    g_signal_connect(toolbar_close, "clicked", G_CALLBACK(DisconnectCallback), NULL);

    // connect signals to one-time UI elements
    g_signal_connect(quit_button, "clicked", G_CALLBACK(destroy), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);


    // NOTE: This is how you populate the status bar with text.
    // First, you need to generate a context ID with the proper context
    // ("none" in our case):
    guint ctx = gtk_statusbar_get_context_id(status_bar, "none");
    // Then, if you KNOW something else is on the status bar, we pop it:
    // (no need to do it here)
    // gtk_statusbar_pop(status_bar, ctx);
    // Finally, we push the new data:
    gtk_statusbar_push(status_bar, ctx, "Not connected");
    // Notice that we must push C strings (const char *);
    // GTK+ itself doesn't understand C++ strings.

    gtk_widget_show_all(window);

    gtk_main();

    return 0;


}

