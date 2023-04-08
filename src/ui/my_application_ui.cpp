// include the generated file
#include "my_application_ui.h"

int main() {
    // Let's instantiate our window
    auto todo_app = MainWindow::create();

    // let's create a model:
    auto todo_model = std::make_shared<slint::VectorModel<TodoItem>>(std::vector {
        TodoItem { false, "Write documentation" },
    });
    // set the model as the model of our view
    todo_app->set_todo_model(todo_model);

    // let's connect our "add" button to add an item in the model
    todo_app->on_todo_added([todo_model](const slint::SharedString& s) {
        todo_model->push_back(TodoItem { false, s });
    });

    // Show the window and run the event loop
    todo_app->run();
}