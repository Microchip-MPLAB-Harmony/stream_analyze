lisp;
'(trace-serial t)


(defun sensor-stream-+ (FNO sensorvector R)  
 (SENSORSTREAM sensorvector (FUNCTION
                (LAMBDA (ROW)
           (OSQL-RESULT sensorvector ROW)))))

(defglobal global_net nil)

(defun same-classify-+ (fno v r)
  (osql-result v (sann-classify global_net v)))   

:osql

_enable_slog_compiler(false);

create function flatten(Vector of Vector w) -> Vector
  as select vector of x
       from Number x, Vector v
      where v in w
        and x in v;
        
create function sensor:stream(Vector of Integer sensornr) -> stream of vector
    as foreign 'sensor-stream-+';
    
create function same_classify(Vector of Number obs) ->  vector
    as foreign 'same-classify-+';

create function detect_anomaly_sann_raw_simple(Stream raw) ->  vector
    as select prediction    
            from
            vector prediction,
            vector of number v
    where v in  winagg(raw,200,40)
    and prediction = same_classify( v);

create function serial_southbound_options_flow(Integer port, Integer baudrate, charstring flow) -> Record
  as select {
    "port": port,
    "baudrate": baudrate,
    "flow_control": flow
  };    
  
create function make_global_net(Integer netnr) -> Integer
 as foreign 'SAME-MAKE-GLOBAL-NET-+';

create function send_sann_serial(Charstring name) ->Boolean
    as foreign 'send-sann-serial-+';


//Use this if the connection require DSR/DTR
//microedge_relay("edge1", southbound_config("SERIAL-COM", 5, serial_southbound_options_flow(15, 115200,"DSR/DTR")));


//Change the first argument to the function serial_southbound_options to match the com-port to the motor-control board. In this case it is 10.
microedge_relay("edge1", southbound_config("SERIAL-COM", 5, serial_southbound_options(${SA_ENGINE_HOST_PORT_NUMBER}, ${SA_ENGINE_BAUD_RATE})));

