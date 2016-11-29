#include "listaDeLobbys.h"
#include <iostream>

ListaDeLobbys::ListaDeLobbys(BaseObjectType* cobject,   
            const Glib::RefPtr<Gtk::Builder>& refGlade) : 
  Gtk::TreeView(cobject), m_builder(refGlade) { 
    
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    set_model(m_refTreeModel);
    append_column("Nombre", m_Columns.m_col_name);
}

ListaDeLobbys::ListaDeLobbys(){}

ListaDeLobbys::~ListaDeLobbys(){}

void ListaDeLobbys::setLista(std::vector<std::string> listaLobbys){
  for(int i=0; i<listaLobbys.size(); ++i){
    Gtk::TreeModel::Row row = *(m_refTreeModel->append());
    row[m_Columns.m_col_name] = listaLobbys[i];
  }
}

std::string ListaDeLobbys::obtenerLobbySeleccionado(){
  auto refSelection = get_selection();
  if(refSelection)
  {
    Gtk::TreeModel::iterator iter = refSelection->get_selected();
    if(iter) {
      Glib::ustring s = (*iter)[m_Columns.m_col_name];
      return std::string(s);
    }
  }
  return "";
}

void ListaDeLobbys::reestablecer(){
  /*m_refTreeModel = Gtk::ListStore::create(m_Columns);
  set_model(m_refTreeModel);*/
}