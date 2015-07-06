CONFIG += debug silent

QT += core xml xmlpatterns

INSTALL_PATH = /usr
message(installing to ($$INSTALL_PATH))

DEFINES+=BOOST_SIGNALS_NO_DEPRECATION_WARNING
