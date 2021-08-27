import cv2
import tensorflow.keras
import numpy as np
import serial
import keyboard


## 이미지 전처리
def preprocessing(frame):
    # 높이, 너비 받아옴
    width = frame.shape[0]
    height = frame.shape[1]

    # 시작 x, y좌표 설정
    x = width//2 - 112
    y = height//2 - 112

    # 중앙에 있는 사진만 잘라옴
    frame_reshaped = frame[x:x+224, y:y+224]
    return frame_reshaped
    

def ObjectIdentification(v, num, cam, port):
    # ser = serial.Serial(port, baudrate = 9600)                    #시리얼 포트 설정
    ## 학습된 모델 불러오기
    model_filename = 'keras_model.h5'
    model = tensorflow.keras.models.load_model(model_filename, compile=False)

    # 카메라 캡쳐 객체, 0=내장 카메라
    capture = cv2.VideoCapture(cam)

    # 캡쳐 프레임 사이즈 조절
    capture.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
    capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)
    while True:
        ret, frame = capture.read()
        if ret == True:
            # 데이터 전처리
            preprocessed = preprocessing(frame)
            
            # 이미지 출력
            cv2.imshow("VideoProcessed", preprocessed)
            
            # 예측
            prediction = model.predict(preprocessed)
            #print(prediction) # [[0.00533728 0.99466264]]

            # 예측 결과 중 가장 높은 결과 선택
            max_index = np.argmax(prediction[0])
            result = 'S'
            arr = ['W', 'S', 'A', 'D']
            result = arr[max_index]

            #num개씩 받는 부분
            if v == num:
                serval = arr[dir_list.index(max(dir_list))]
                serval = serval.encode()
                #인코딩 방식 새로 추가
                serval = (str(arr[max_index])+'\n').encode("utf-8")

                # 출력
                # ser.write(serval)                                     #시리얼 출력
                print(serval, ': ', prediction[0, max_index]*100, '%')
            else:
                for i in range(4):              #Ilovepython
                    if result == arr[i]:
                        dir_list[i] += 1

            # sec초마다 검사하며, videoframe 창으로 아무 키나 누르게 되면 종료
            msec = 1   
            if cv2.waitKey(msec) == 27:  #원래 200
                break
        else:
            print("Wait...")

        v += 1
        if v > num:
            v = 0
        
    # 카메라 객체 반환
    capture.release() 
    # 화면에 나타난 윈도우들을 종료
    cv2.destroyAllWindows()
    

"""AI 고카트 TeachableMachine 모델 시리얼 출력 프로그램"""
#num개씩 나눠서 출력하는 리스트
dir_list = [0, 0, 0, 0]     #direction list, 방향별로 얻은 값이 있다면 ++     
v = 0                       #반복 횟수 저장
num = 10                    #받을 값 수
cam = 1                     #WebCam 번호
port = 'COM3'               #시리얼 포트 번호

ObjectIdentification(v, num, cam, port)