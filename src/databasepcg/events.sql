
CREATE TABLE [dbo].[Events] (
	[EventID] [int] IDENTITY (1, 1) NOT NULL ,
	[Event] [int] NOT NULL ,
	[DateTime] [datetime] NULL ,
	[ComputerName] [varchar] (15) NOT NULL ,
	[Subject] [int] NOT NULL ,
	[SubjectName] [varchar] (32) NOT NULL ,
  [Context] [varchar] (24) NOT NULL ,
	[ObjectRef] [varchar] (4000) NOT NULL
)
GO

CREATE Trigger EventDateTime
On dbo.Events
For Insert, Update
As
	UPDATE Events Set DateTime=GetDate()
	WHERE Events.IDENTITYCOL IN (SELECT IDENTITYCOL FROM inserted)


GO
