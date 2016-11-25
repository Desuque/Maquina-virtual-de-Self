#ifndef LISTADELOBBYS_H
#define LISTADELOBBYS_H

#include <vector>
#include <gtkmm.h>

class ListaDeLobbys : public Gtk::TreeView {

public:
  ListaDeLobbys();
  ListaDeLobbys(BaseObjectType* cobject,   
            const Glib::RefPtr<Gtk::Builder>& refGlade); 
  ListaDeLobbys(BaseObjectType* cobject,   
            const Glib::RefPtr<Gtk::Builder>& refGlade,
             bool is_glad); 
  
  virtual ~ListaDeLobbys();
  void reestablecer();
  void on_button_ok();
  void setLista(std::vector<std::string> listaLobbys);
  std::string obtenerLobbySeleccionado();

protected:
  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns(){ 
      add(m_col_name); 
    }
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
  };

  ModelColumns m_Columns;
  Gtk::Button m_Button_Ok;

  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
  Glib::RefPtr<Gtk::Builder> m_builder;
};

#endif 