#include "listaDeLobbys.h"
#include <iostream>
ListaDeLobbys::ListaDeLobbys(BaseObjectType* cobject,   
            const Glib::RefPtr<Gtk::Builder>& refGlade) : 
  Gtk::TreeView(cobject),
  m_builder(refGlade) { 

    //ListaDeLobbys* listaDeLobbys = nullptr; 
    //m_builder->Gtk::Builder::get_widget("treeview1", listaDeLobbys);
    //std::vector<std::string> names = {"lobby1", "lobby2"};
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    set_model(m_refTreeModel);

    /*Gtk::TreeModel::Row row = *(m_refTreeModel->append());
    row[m_Columns.m_col_name] = "lobby1";

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_name] = "lobby2";

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_name] = "lobby3";*/

    append_column("Nombre", m_Columns.m_col_name);
    
    Gtk::Button* buttonOk = nullptr; 
    m_builder->Gtk::Builder::get_widget("button12", buttonOk);
    buttonOk->signal_clicked().connect(sigc::mem_fun(*this,&ListaDeLobbys::on_button_ok));
    //m_Button_Ok.signal_clicked().connect( sigc::mem_fun(*this,
      //                    &ListaDeLobbys::on_button_ok) );
}

ListaDeLobbys::ListaDeLobbys(BaseObjectType* cobject, 
      const Glib::RefPtr<Gtk::Builder>& refGlade,
      bool is_glad) : ListaDeLobbys(cobject, refGlade) {
}

ListaDeLobbys::ListaDeLobbys(){
}

ListaDeLobbys::~ListaDeLobbys()
{
}

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

void ListaDeLobbys::on_button_ok(){
  /*auto refSelection = get_selection();
  if(refSelection)
  {
    Gtk::TreeModel::iterator iter = refSelection->get_selected();
    if(iter) {

      std::cout << (*iter)[m_Columns.m_col_name] <<  std::endl;
      Glib::ustring s = (*iter)[m_Columns.m_col_name];
      std::string s2 = s;
       std::cout << s2 <<  std::endl;
    }
  }
  hide();*/
}