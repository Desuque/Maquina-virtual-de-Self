 # build an executable named myprog from myprog.c
conConexion: 
	g++ -ggdb -O0 server_ejemplo_con_conexion.cpp server_boolean.cpp server_check_point.cpp server_div.cpp \
	server_equal.cpp server_int.cpp server_json_writer.cpp \
	server_mult.cpp server_not_equal.cpp server_object.cpp server_server.cpp \
	server_slot.cpp server_string.cpp server_sub.cpp server_sum.cpp server_virtual_machine.cpp \
	server_self_writer.cpp ../interfaz/common_proxyClient.cpp ../interfaz/common_socket.cpp  \
	../interfaz/common_proxy.cpp client_interface_slot.cpp  \
	client_json_reader.cpp -o server

sinConexion: 
	g++ -ggdb -O0 server_examples.cpp server_boolean.cpp server_check_point.cpp server_div.cpp \
	server_equal.cpp server_int.cpp   server_json_writer.cpp \
	server_mult.cpp server_not_equal.cpp server_object.cpp server_server.cpp \
	server_slot.cpp server_string.cpp server_sub.cpp server_sum.cpp server_virtual_machine.cpp \
 	server_self_writer.cpp ../interfaz/client_interface_slot.cpp ../interfaz/client_json_reader.cpp \
 	 -o server

clean: 
	$(RM) server