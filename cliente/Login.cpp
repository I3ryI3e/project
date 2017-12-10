

#include "Login.h"
#include <QString>
#include <string>
#include "dados.h"
#include <string>




Login::Login(){
    widget.setupUi(this);
    widget.submitbutton->hide();
    widget.errortext->hide();
    widget.passwordtext->setEchoMode(QLineEdit::Password);
    show();
}


Ui::Login Login::getWidget(){
    return widget;
}

Login::~Login() {
}
