 # build an executable named myprog from myprog.c
conConexion: 
	g++ -ggdb -O0 -std=c++11 -pthread server_parser.cpp server_null_linker.cpp server_linker.cpp server_main.cpp server_boolean.cpp server_div.cpp server_nil.cpp server_error.cpp \
	server_equal.cpp server_int.cpp server_float.cpp server_app.cpp server_garbage_slot.cpp server_json_writer.cpp \
	server_mult.cpp server_not_equal.cpp common_thread.cpp server_object.cpp server_server.cpp \
	server_slot.cpp server_string.cpp server_sub.cpp server_sum.cpp server_virtual_machine.cpp  \
	server_self_writer.cpp common_proxyClient.cpp ../interfaz/socket.cpp  \
	 ../interfaz/client_interface_slot.cpp  \
	../interfaz/client_json_reader.cpp -o server

sinConexion: 
	g++ -ggdb -O0 -std=c++11 server_parser.cpp server_null_linker.cpp server_linker.cpp  server_boolean.cpp  server_div.cpp \
	server_equal.cpp server_int.cpp   server_float.cpp server_json_writer.cpp server_garbage_slot.cpp \
	server_mult.cpp server_not_equal.cpp server_object.cpp server_server.cpp \
	server_slot.cpp server_string.cpp server_sub.cpp server_sum.cpp server_virtual_machine.cpp \
 	server_self_writer.cpp ../interfaz/client_interface_slot.cpp ../interfaz/client_json_reader.cpp \
 	 -o server

clean: 
	$(RM) server
