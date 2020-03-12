#include "Manager.h"


Manager::Manager() : view(std::make_shared<ViewHolder>(
                std::make_shared<MenuView>(*this))){

}

void Manager::startNewGame() {
    view->reset(); //Clear view itself, not the holder
    *view = std::make_shared<NcursesView>(*this);
}
