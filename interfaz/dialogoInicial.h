#ifndef DIALOGOINICIAL_H
#define DIALOGOINICIAL_H

#include <gtkmm.h>

class DialogoInicial : public Gtk::Dialog {
	protected:
		Glib::RefPtr<Gtk::Builder> m_builder;
	
	public:	
		DialogoInicial();
		DialogoInicial(BaseObjectType* cobject, 	
					const Glib::RefPtr<Gtk::Builder>& refGlade);
		DialogoInicial(BaseObjectType* cobject, 
					const Glib::RefPtr<Gtk::Builder>& refGlade,
					bool is_glad);
		void nuevoLobbyClick();
		void cargarLobbyClick();
		virtual ~DialogoInicial();
};

#endif