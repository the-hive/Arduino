
#define BANKPERIOD 5000

#define PINCOUNT 4
const int pin[] = { 3, 4, 8, 9 };

#define LEDCOUNT 12
int vidmem[ LEDCOUNT ];

const int banks[][PINCOUNT-1] = {
  { 0, 2, 4 },
  { 1, 6, 8 },
  { 3, 7,10 },
  { 5, 9,11 }
};
const int ledH[] = { 1, 0, 2, 0, 3, 0, 2, 1, 3, 1, 3, 2 };

void setup()
{
  for( int i = 0; i < PINCOUNT; ++i )
    pinMode( pin[i], INPUT );
    
  for( int i = 0; i < LEDCOUNT; ++i )
    vidmem[i] = 0;
}

void outputUpdate()
{
  static unsigned long nextBank = micros();
  if ( micros() > nextBank )
  {
    nextBank += BANKPERIOD;
    
    static int iBank = 0;
    if ( ++iBank >= PINCOUNT )
      iBank = 0;

    for( int i=0; i< PINCOUNT; ++i )
      pinMode( pin[i], INPUT );

    pinMode( pin[ iBank ], OUTPUT );
    digitalWrite( pin[ iBank ], LOW );
    
    for( int iSub = 0; iSub < PINCOUNT-1; ++iSub )
    {
      if ( vidmem[ banks[iBank][iSub] ] != 0 )
      {
        pinMode( pin[ ledH[ banks[iBank][iSub]]], OUTPUT );
        digitalWrite( pin[ ledH[ banks[iBank][iSub]]], HIGH );
      }
    }
  }
}

#define MODECOUNT 2
void fillUp( bool init )
{
  static int p = 0;
  static unsigned long nextFrame = millis();

  if ( init )
  {
    for( int i = 0; i < LEDCOUNT; ++i )
      vidmem[i] = 0;
      p = 0;
      nextFrame = millis();
  }

  if ( millis() > nextFrame )
  {
    nextFrame += 75; //   <<<<------   period

    vidmem[p] = vidmem[p] ? 0 : 255;
    ++p;
    if ( p >= LEDCOUNT )
      p = 0;
  }
}

void randomBlinky( bool init )
{
  static unsigned long nextFrame = millis();
  if ( init )
  {
    nextFrame = millis();
  }
  
  if ( millis() > nextFrame )
  {
    nextFrame += 50;
    
    int r = random( LEDCOUNT );
    vidmem[r] = vidmem[r] ? 0 : 255;
  }
}

void cylon( bool init )
{
  static unsigned long nextFrame = millis();
  static int frameNumber = 0;
  if ( init )
  {
    nextFrame = millis();
  }

  static int frames[] = { 0,0,0,1,1,2,3,4,5,6,6,7,8,9,10,10,11,11,11,11,11,10,10,9,8,7,6,6,5,4,3,2,1,1,0,0,-1};
  if ( millis() > nextFrame )
  {
    nextFrame += 25;
    
    vidmem[frames[frameNumber]] = 0;
    if( frames[++frameNumber] == -1 )
      frameNumber = 0;
    vidmem[frames[frameNumber]] = 255;
  }

}

#define MODECOUNT 3
void loop()
{
  outputUpdate();
  
  static int mode = 1;
  static unsigned long nextModeChange = millis();
  bool newMode = false;
  if ( millis() > nextModeChange )
  {
    nextModeChange += 10000;
    ++mode;
    if( mode == MODECOUNT )
      mode = 0;
    newMode = true;
  }
    
  switch( mode )
  {
    case 0:
      fillUp( newMode );
      break;
    case 1:
      randomBlinky( newMode );
      break;
    case 2:
      cylon( newMode );
      break;
  }  
}
