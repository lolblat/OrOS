//
// Created by lolblat on 05/01/18.
//
void TestFunction()
{

}

void main()
{
    char *video_driver = (char*)0xb8000;
    *video_driver = 'X';
    TestFunction();
}