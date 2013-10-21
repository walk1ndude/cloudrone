var ardrone2 = {
    'name' : 'Ar.Drone 2.0',
    'photo' : '../images/drones/ardrone2.jpg',
    'topics': {
        'navdata' : {
           'topic' : '/ardrone/navdata',
           'messageType' : 'ardrone_autonomy/Navdata',
            'data' : {
                'altd' : 'высота', 
                'vx' : 'скорость по x',
                'vy' : 'скорость по y',
                'vz' : 'скорость по z',
                'ax' : 'ускорение по x',
                'ay' : 'ускорение по y',
                'az' : 'ускорение по z',
                'rotX' : 'поворот по оси x',
                'rotY' : 'поворот по оси y',
                'rotZ' : 'поворот по оси z',
                'batteryPercent' : 'заряд батареи',
                'magX' : 'магнетометр по x',
                'magY' : 'магнетометр по y',
                'magZ' : 'магнетометр по z',
                'pressure' : 'давление',
                'state' : {
                    'name' : 'cостояние',
                    'states' : [
                        'не определен',
                        'инициализирован',
                        'на земле',
                        'в полете',
                        'зависание',
                        'тест',
                        'взлет',
                        'полет в точку',
                        'посадка',
                        'цикл'
                            ]
                }
            }
        },
        'video' : {
            'topic' : '/cloudrone/image_raw',
            'width' : 640,
            'height' : 360
        }
    },
    'specs' : {
        'Фронтальная камера' : '640x360 или 1280x720 30 fps',
        'Вертикальная камера' : '320x240 60 fps',
        'Батарея' : 'Li-Po 1000 mah',
        'Процессор' : '1GHz 32 bit ARM Cortex A8',
        'Оперативная память' : '1GB DDR2 RAM 200MHz',
        'Акселерометр' : '3-х осевой, точность +/- 50 мг',
        'Гироскоп' : '3-х осевой, 2000 градусов/сек',
        'Магнетометр' : '3-х осевой, точность +/- 6 градусов',
        'Двигатели' : '4 бесщеточных мотора, 14.5 Вт, 28.500 вращений в минуту',
        'Масса' : '380 г или 420 г - в зависимости от корпуса',
        'Операционная система' : 'Linux 2.6.32',
        'Барометр' : 'точность +/-10 Па',
        'Способ коммуникации': '802.11 b/g/n'
    }
}
