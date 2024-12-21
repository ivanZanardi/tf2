#define paste(x)        x
#define append_us(x)    paste(x)_
#define append_y(x,y)   append_us(x)y
#define concat(x, y)    append_y(x,y)

#define read_csv_       concat(read_csv,t)
#define write_csv_      concat(write_csv,t)


subroutine read_csv_(filename, row_lim_in, col_lim_in, delimiter_in, dset)
  ! Declarations
  ! -----------------------------------
  ! Declare in-out variables
  character(*), intent(in) :: filename
  integer, intent(in), optional, dimension(2) :: row_lim_in
  integer, intent(in), optional, dimension(2) :: col_lim_in
  character(*), intent(in), optional :: delimiter_in
  tt, intent(out), allocatable, dimension(:,:) :: dset
  ! Declare local variables
  integer, parameter :: huge = int(1.d5)
  integer :: i, j, c, r, n, m, rc, fu
  character(len=huge) :: line
  tt, allocatable, dimension(:) :: row_dset
  ! Optional arguments
  integer, dimension(2) :: row_lim = (/1,huge/)
  integer, dimension(2) :: col_lim = (/1,huge/)
  character(1) :: delimiter = ','
  ! Optional arguments
  ! -----------------------------------
  if (present(row_lim_in)) row_lim = row_lim_in
  if (present(col_lim_in)) col_lim = col_lim_in
  if (present(delimiter_in)) delimiter = delimiter_in
  ! File handling
  ! -----------------------------------
  open(                               &
    action='read',                    &
    file=filename,                    &
    newunit=fu,                       &
    access='sequential',              &
    form='formatted',                 &
    status='old'                      &
  )
  ! Count lines
  n = 0
  m = 0
  do
    read(fu, '(A)', iostat=rc) line
    if ((m == 0).and.(n > row_lim(1))) then
      m = columns_count(trim(line), trim(delimiter))
    endif
    if (rc /= 0) exit
    n = n + 1
  end do
  ! Rewind file
  rewind(fu)
  ! Allocate variables
  ! -----------------------------------
  allocate(dset(                      &
    min(n, row_lim(2)-row_lim(1)),    &
    min(m, col_lim(2)-col_lim(1))     &
  ))
  allocate(row_dset(m))
  ! Read values
  ! -----------------------------------
  r = 1
  do i = 1,n
    c = 1
    if ((row_lim(1) <= i).and.(i < row_lim(2))) then
      read(fu, *) row_dset
      do j = 1,m
        if ((col_lim(1) <= j).and.(j < col_lim(2))) then
          dset(r,c) = row_dset(j)
          c = c + 1
        endif
      enddo
      r = r + 1
    else
      read(fu, *) line
    endif
  enddo
  close(fu)
end subroutine read_csv_

subroutine write_csv_(filename, n, dset)
  ! Declarations
  ! -----------------------------------
  ! Declare in-out variables
  character(*), intent(in) :: filename
  integer, intent(in) :: n
  tt, intent(in), dimension(:,:) :: dset
  ! Declare local variables
  integer :: i, fu
  ! Core function
  ! -----------------------------------
  open(                               &
    action='write',                   &
    file=filename,                    &
    newunit=fu                        &
  )
  do i = 1,n
    write(fu, *)  dset(i,:)
  enddo
  close(fu)
end subroutine write_csv_


#undef t
#undef tt

#undef paste
#undef append_us
#undef append_y
#undef concat

#undef read_csv_
#undef write_csv_
