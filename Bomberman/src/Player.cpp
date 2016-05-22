#include "Player.h"
#include "Map.h"
#include "BombManager.h"

Player::Player(SDL_Texture* tex, unsigned int tile_size, Relay *relay,
               KeyboardInput *keyboard_input, unsigned int player_id, unsigned int val_x, unsigned int val_y)
{
    this->m_x = val_x*tile_size;
    this->m_y = val_y*tile_size;

    this->m_tex = tex;
    this->m_direction = DOWN;
    this->m_status = 0;
    this->m_tile_size = tile_size;

    m_timer.ResetTimer();
    m_player_size_w = m_player_size_w *tile_size/32;
    m_player_size_h = m_player_size_h *tile_size/32; //size according to 32px tile size
    m_move_speed = m_move_speed *m_tile_size/32; //speed according to 32px tile size
    m_relay = relay;
    m_keyboard_input = keyboard_input;
    m_player_id = player_id;
}

void Player::Update()
{
    if(m_timer.GetTimeElapsed()>m_speed)
    {
        //Key Input
        if(m_keyboard_input->IsKeyOn(m_bomb_button))
         {
             m_bomb_ready = true;
         }
         else if(!m_keyboard_input->IsKeyOn(m_bomb_button) && m_bomb_ready)
             {
                 this->PlaceBomb();
                 m_bomb_ready = false;
             }
             else if(m_keyboard_input->IsKeyOn(m_up))
                    {
                        this->PlayerMove(0, -1);
                    }
                    else if(m_keyboard_input->IsKeyOn(m_down))
                        {
                            this->PlayerMove(0, 1);
                        }
                        else if(m_keyboard_input->IsKeyOn(m_left))
                            {
                                this->PlayerMove(-1, 0);
                            }
                            else if (m_keyboard_input->IsKeyOn(m_right))
                                 {
                                    this->PlayerMove(1, 0);
                                 }
                                else
                                {
                                    this->SetStatus(0);
                                }
    }

    while(m_timer.GetTimeElapsed()>m_speed)
    {
        m_timer.DecreaseTimer(m_speed);
    }

}

void Player::PlaceBomb()
{
   m_relay->GetBombManager()->MakeBomb(5000,m_x+m_player_size_w/2,m_y+m_player_size_h/2,2.5); // TODO Fix testing values
}

void Player::Draw(SDL_Renderer *renderer)
{
    SDL_Rect SrcR;
    SDL_Rect DestR;

    unsigned int SHAPE_SIZE_x;
    unsigned int SHAPE_SIZE_y;

    unsigned int source_x;
    unsigned int source_y;
    unsigned int source_const;
    unsigned int texture_offset;
    unsigned int main_offset = 568;
    //----------------------
    SHAPE_SIZE_x = 18;
    SHAPE_SIZE_y = 32;

    source_x = m_player_id*main_offset+3; //Calculates second set of images
    source_y = 4;
    source_const = 4;
    texture_offset = source_const + SHAPE_SIZE_x + 2;

    SrcR.y = source_y;
    SrcR.w = SHAPE_SIZE_x;
    SrcR.h = SHAPE_SIZE_y;
    //----------------------

    switch(m_direction)
    {
    case UP:
        //POSTION OF BOMBERMAN
        if(m_status % 3 == 0)
        {
            SrcR.x = source_x + 3*texture_offset;
        }
        if(m_status % 3 == 1)
        {
            SrcR.x = source_x + 4*texture_offset;
        }
        if(m_status % 3 == 2)
        {
            SrcR.x = source_x + 5*texture_offset;
        }
        break;

    case DOWN:
        //POSTION OF BOMBERMAN
        if(m_status % 3 == 0)
        {
            SrcR.x = source_x;

        }
        if(m_status % 3 == 1)
        {
            SrcR.x = source_x + texture_offset;
        }
        if(m_status % 3 == 2)
        {
            SrcR.x = source_x + 2*texture_offset;
        }
        break;

    case LEFT:
        //POSTION OF BOMBERMAN
        if(m_status % 3 == 0)
        {
            SrcR.x = source_x + 6*texture_offset;
        }
        if(m_status % 3 == 1)
        {
            SrcR.x = source_x + 7*texture_offset;
        }
        if(m_status % 3 == 2)
        {
            SrcR.x = source_x + 8*texture_offset;
        }
        break;

    case RIGHT:
        //POSTION OF BOMBERMAN
        if(m_status % 3 == 0) //POSTION OF BOMBERMAN
        {
            SrcR.x = source_x + 9*texture_offset;
        }
        if(m_status % 3 == 1)
        {
            SrcR.x = source_x + 10*texture_offset;
        }
        if(m_status % 3 == 2)
        {
            SrcR.x = source_x + 11*texture_offset;
        }
        break;
    }

    //----------------------
    DestR.x = m_x;
    DestR.y = m_y;
    DestR.w = m_player_size_w;
    DestR.h = m_player_size_h;
    //----------------------

    SDL_RenderCopy(renderer, m_tex, &SrcR, &DestR);
}

void Player::PlayerMove(int x, int y)
{
    unsigned int field_size = m_tile_size;

    if(x == 1) // MOVE RIGHT ------------
    {
        m_direction = RIGHT;
        if( m_relay->GetMap()->Walkable( m_y/field_size, (m_x+m_player_size_w+m_move_speed)/field_size )
                && m_relay->GetMap()->Walkable( (m_y+m_player_size_h)/field_size, (m_x+m_player_size_w+m_move_speed)/field_size) )
        {
            m_x = m_x + m_move_speed;
        }
    }
    else if (x == -1) // MOVE LEFT ------------
        {
            m_direction = LEFT;
            if(m_relay->GetMap()->Walkable( m_y/field_size, (m_x-m_move_speed)/field_size )
                    && m_relay->GetMap()->Walkable( (m_y+m_player_size_h)/field_size, (m_x-m_move_speed)/field_size) )
            {
                m_x = m_x - m_move_speed;
            }
        }
        else if(y == 1) // MOVE DOWN ------------
            {
                m_direction = DOWN;
                if(m_relay->GetMap()->Walkable( (m_y+m_player_size_h+m_move_speed)/field_size, m_x/field_size )
                    && m_relay->GetMap()->Walkable( (m_y+m_player_size_h+m_move_speed)/field_size, (m_x+m_player_size_w)/field_size ) )
                {
                    m_y = m_y + m_move_speed;
                }
            }
            else if(y == -1) // MOVE UP ------------
                {
                    m_direction = UP;
                    if(m_relay->GetMap()->Walkable( (m_y - m_move_speed)/field_size, m_x/field_size )
                        &&  m_relay->GetMap()->Walkable( (m_y - m_move_speed)/field_size, (m_x+m_player_size_w)/field_size) )
                    {
                        m_y = m_y - m_move_speed;
                    }
                }
    m_status++;
    if(m_status > 2)
        m_status = 0;
}

unsigned int Player::GetX() const
{
    return m_x;
}

void Player::SetX(unsigned int val)
{
    m_x = val;
}

unsigned int Player::GetY() const
{
    return m_y;
}

void Player::SetY(unsigned int val)
{
    m_y = val;
}

unsigned int Player::GetSizeW() const
{
    return m_player_size_w;
}

unsigned int Player::GetSizeH() const
{
    return m_player_size_h;
}

unsigned int Player::GetStatus() const
{
    return m_status;
}

void Player::SetStatus(unsigned int s)
{
    m_status = s;
}

unsigned int Player::GetDirection() const
{
    return m_direction;
}

void Player::SetDirection(int d)
{
    switch(d)
    {
        case 0:
            m_direction = LEFT;
            break;
        case 1:
            m_direction = RIGHT;
            break;
        case 2:
            m_direction = UP;
            break;
        case 3:
            m_direction = DOWN;
            break;
        default:
            m_direction = DOWN;
    }
}

unsigned int Player::GetHealth() const
{
    return m_health;
}

void Player::SetHealth(unsigned int h)
{
    m_health = h;
}

void Player::SetKeycodes(SDL_Keycode up, SDL_Keycode down, SDL_Keycode left, SDL_Keycode right, SDL_Keycode bomb_button)
{
    m_up = up;
    m_down = down;
    m_left = left;
    m_right = right;
    m_bomb_button = bomb_button;
}


unsigned int Player::GetAlive() const
{
    return m_alive;
}

void Player::SetAlive(unsigned int a)
{
    m_alive = a;
}

unsigned int Player::GetLives() const
{
    return m_lives;
}

void Player::SetLives(unsigned int l)
{
    if(l < 0)
        m_lives = 3;
    else
        m_lives = l;
}

unsigned int Player::GetID() const
{
    return m_player_id;
}